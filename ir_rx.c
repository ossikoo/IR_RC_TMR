#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "generic.h"
#include "usart.h"
#include "owndelay.h"
#include "ir_recode.h"
#include "ir_handler.h"
#include "ir_rx.h"
#include "rtc.h"

uint8_t upper_cnt;
uint8_t ir_start_rx=1;
volatile uint8_t new_val=1;
uint16_t timediff;
uint16_t ir_rx_cnt = 0;

uint16_t endtime =0xffff;

void init_ir_rx(void)
{
  PRR &= ~(1 << PRTIM1); //Enable timer1

  PORTB = 0;
  set_input(DDRB, IR_RX);

  cli();
  
  TCCR1A = 0;
  TCCR1A &= !((1<<WGM10));  
  TCCR1A &= !((1<<WGM11));
  TCCR1B |=  ((1<<WGM12));
  TCCR1B &= !((1<<WGM13));

  TCCR1B &= ~((1<<CS11));
  TCCR1B |=  ((1<<CS10) | (1<<CS12));
  TCCR1B &= ~((1<<ICES1));

  TCCR1C = 0;
  TIMSK1 |= ((1<<ICIE1) | (1<<TOIE1));
  OCR1A = 0;
  OCR1B = 14395; //one second
  TCNT1 = 15500;

  upper_cnt = 0;
  
  //There might be hanging compare match or overflow, make sure there aint
  //TIFR1 |= ((1<<ICF1) | (1<<TOV1));
  TIMSK1 |= ((1<< ICIE1) | (1<<TOIE1) | (1<<OCIE1A) | (1<<OCIE1B));
  sei();
}

uint32_t ir_recv(uint16_t *ir_time_buf, uint8_t *len)
{
  uint32_t ret = -1;
  uint8_t i;
  uint16_t bit_cnt = 0;
  uint8_t triplet_val[IR_TRIPLET_BUF_SZ] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  
  init_ir_rx();

  for(i=0;i<IRCODE_BUF_SZ-1;i++)
    ir_time_buf[i] = 0;
  
  while(1) {
    
    if(new_val)
      {	
	uint8_t val;
	uint8_t byte_addr;
	uint8_t bit_addr;
	val = ir_time_diff_decode(timediff);
	printf("%d ",timediff);
	timediff=0;
	switch(val) {
	case 0:
        case 1:
	  byte_addr = bit_cnt / 8;
	  bit_addr = bit_cnt % 8;
	  if (byte_addr < IR_TRIPLET_BUF_SZ)
	    {
	      triplet_val[byte_addr] |= (val & 0x01) << bit_addr;
	      //triplet_val |= ((val & 0x01) << bit_cnt);
	    } else {	 
	  }
	  bit_cnt++;
	  if(bit_cnt % 24 == 0) {
	    
	    printf(" %02hx %02hx %02hx",triplet_val[byte_addr], \
		   triplet_val[byte_addr-1], triplet_val[byte_addr-2]);
	    //	    triplet_val[0] = 0;
	    //	    triplet_val[1] = 0;
	    //	    triplet_val[2] = 0;
	    //	    bit_cnt = 0;
	  }
	  break;	  
	case IR_START:
	  bit_cnt=0;	  
	  printf ("IR_START ");
	  break;
	case IR_END:
	  printf ("IR_END(%d) ",ir_rx_cnt);
	  handle_ir(bit_cnt, (uint8_t*)triplet_val);
	  ir_rx_cnt=0;
	  for(i=0;i<IR_TRIPLET_BUF_SZ-1;i++)
	    triplet_val[i] = 0;
	  break;
	case IR_CONT:
	  printf ("IR_CONT ");
	  break;
	case IR_REP:
	  printf ("IR_REP ");
	  break;
	case IR_STOP:
	  printf ("IR_STOP ");
	  break;
	default:
	  printf(",%hx\n", val);
	}
	new_val = 0;
      }

    if(timediff > endtime)
      new_val = 1;
    // output_high(PORTC, LED);      
    
    //output_low(PORTC, LED);
    
    /*    
	  if(ir_decode_now == 1) //|| ir_rx_cnt >= IRCODE_BUF_SZ*)
	  {
	  ir_decode_now = 0;
	  ir_start_rx = 1;
	  output_high(PORTC, LED);
	  
	  ret = ir_time_buf_decode(ir_time_buf, &ir_rx_cnt);
	  
	  *len = ir_rx_cnt;
	  
	  output_low(PORTC, LED);
	  
	  ir_rx_cnt = 0;
	  break; //ir rx ready
	  }
    */
  }
  
  return ret;
}

ISR(TIMER1_OVF_vect)
{
  //TCNT1 = 15500;
  printf("%d\n",upper_cnt);
  upper_cnt++;  
}

ISR(TIMER1_COMPA_vect)
{
  //Generating IR code end mark
  if(ir_rx_cnt) {
    new_val=1;
    timediff=650;
  }
}

ISR(TIMER1_COMPB_vect)
{
  uint32_t next_val;
  next_val = OCR1B + 14395;
  OCR1B = (next_val & 0x0000ffff);
  rtc_time_tick();
  if(rtc_timer_on())
    {
      //do_sg..
    }
}

ISR(TIMER1_CAPT_vect)
{
  static uint16_t ICR1_prev = 0;
  uint32_t t1;
  uint32_t t2;

  output_high(PORTC, LED);        

  t1 = upper_cnt;
  t1 = (t1 << 16);
  t2 = t1;
  t2 = t2 | ICR1;
  t1 = t1 | ICR1_prev;

  timediff = t2 - t1;
  
  if(ir_start_rx) {
    //reset timer @start
    TCNT1 = 0;
    upper_cnt = 0;
    ir_rx_cnt = 0;
    ir_start_rx = 0;
  }  
  
  ICR1_prev = ICR1;
  OCR1A = ICR1 + IR_MAX_WAIT_TIME;
  //  if(ir_rx_cnt < IRCODE_BUF_SZ)
  //    ir_time_buf[ir_rx_cnt] = (temp >> 2) | (upper_cnt & 0x03) << 6;

  new_val = 1;
  
  ++ir_rx_cnt;

  output_low(PORTC, LED);
}  

