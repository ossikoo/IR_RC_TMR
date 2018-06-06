#include <stdio.h>
#include <avr/io.h>
#include "generic.h"
#include "usart.h"
#include "owndelay.h"
#include "ir_recode.h"
#include "ir_tx.h"

uint16_t IR_COE[IRCODE_BUF_SZ] = {254,255,0,1,0,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,1,254,254};

//38kHz fast PWM
void init_ir_tx(void)
{
  PRR &= ~(1 << PRTIM1); //Enable timer1

  delay_ms(10);

  TCCR1A &= ~(1<<COM1B1); //PB2
  TCCR1A |=  (1<<COM1B0);

  TCCR1A &= ~(1<<COM1A1); //PB1
  TCCR1A |=  (1<<COM1A0);  
  
  TCCR1A &= ~(1<<WGM10);  
  TCCR1A &= ~(1<<WGM11);
  TCCR1B |=  (1<<WGM12);
  TCCR1B |=  (1<<WGM13);

  TCCR1B |=  (1<<CS10);
  TCCR1B &= ~(1<<CS11);
  TCCR1B &= ~(1<<CS12);
  TCCR1B &= ~(1<<ICES1);


  //TCCR1C = 0;
  //TIMSK1 |= ((1<<ICIE1) | (1<<TOIE1));
  ICR1 = 193; //top value
  //  OCR1B = 10000; //compare match
  TCNT1 = 0;
  //TIFR1 |= ((1<<ICF1) | (1<<TOV1));
}

void send_ir_start(void)
{
  PWM_ON;
  delay_10us(900);
  PWM_OFF;
  delay_10us(450);
}

void send_ir_digit(uint8_t digit)
{
  PWM_ON;
  delay_10us(IR_SHORT);
  PWM_OFF;
  if(digit==1)
    delay_10us(IR_LONG);
  else
    delay_10us(IR_SHORT);
}

void send_ir_stop(void)
{
  //  dbg_printf("send_ir_stop\n");
  send_ir_digit(1);
  
}

void send_ir_repeat(void)
{
//Repeat code is +9ms -2,2ms +600us
  PWM_ON;
  delay_10us(900);
  PWM_OFF;
  delay_10us(220);
  send_ir_stop();
}

void ir_send(uint32_t code, uint16_t *ir_time_buf)
{
  uint8_t i;
  ir_make_time_buf(code, ir_time_buf);
  dbg_printf("ir_send: %lx\n", code);
  for(i=0;i<IRCODE_BUF_SZ;i++) {
    dbg_printf(" %d,", ir_time_buf[i]);
  }
  dbg_printf("\n");
  send_ir_tx(ir_time_buf);
  delay_10us(4000);
  send_ir_repeat();
}

void send_ir_tx(uint16_t *ir_time_buf)
{
  uint8_t i;
  
  init_ir_tx();
  
  for(i=0;i<IRCODE_BUF_SZ;i++)
    {
      switch(ir_time_buf[i])
	{
	case 0:
	  send_ir_digit(0);
	  break;
	case 1:
	  send_ir_digit(1);
	  break;
	case IR_NOP:
	  break;
	case IR_STOP:
	  send_ir_stop();	  
	  break;
	case IR_START:
	  send_ir_start();
	  break;
	default:	
	  break;
	}
    }
}

/*
int main(void)
{
  uint16_t *ir_time_buf = NULL;
  uint16_t start=0;
  
  uint16_t ir_rx_cnt=0;
  uint8_t i;
  
  ir_time_buf = (uint16_t*)malloc(sizeof (uint16_t)*IR_TIME_BUF_SZ);
  set_output(DDRC, LED);
  //test loop for 2ms period
  init_ir_tx();
  output_low(PORTB,IR_TX);
  set_output(DDRB,PB1);

  
  // test binary pattern
  //while(1) {
  //  output_high(PORTB,PB2);
  //  delay_10us(900);
  //  output_low(PORTB,PB2);
  //  delay_10us(99);
  //}  

  // test PWM pattern
  //while(1) {
  //  PWM_ON;
  //  delay_10us(900);
  //  PWM_OFF;
  //  delay_10us(99);
  //}

  for(;;)
    {
      send_ir_tx((uint16_t*)&IR_CODE);
      delay_ms(200);
    }
  
  stdout = &mystdout;
  
  init_ir_rx();
  USART_Init();

  for(i=0;i<IR_TIME_BUF_SZ-1;i++)
    ir_time_buf[i] = 0;

  delay_ms(1);
  delay_ms(1);
  delay_ms(1);
  delay_ms(1);
  delay_ms(1);
  delay_ms(1);
  
  
  while (1) {

    // output_high(PORTC, LED);      
    if (TIFR1 & (1<<ICF1)) {
      //    ir_time_buf[ir_rx_cnt] = TCNT1L & 0x00ff;
      //    ir_time_buf[ir_rx_cnt] |= (TCNT1H << 8) & 0xff00;
      uint16_t temp = ICR1;
      ir_time_buf[ir_rx_cnt] = temp;
      output_high(PORTC, LED);      
      TIFR1 |= (1<<ICF1);
      ++ir_rx_cnt;
      //      ++ir_rx_cnt;      
      //      delay_ms(1);
      output_low(PORTC, LED);
      //TCNT1 = 0; //reset timer
      if(!start)
	start = TCNT1;
    }
    //output_low(PORTC, LED);

    if(TIFR1 & (1<<TOV1)) {
      //if(TCNT1H > 10) {
      TIFR1 |= (1<<TOV1);
      //    output_high(PORTC, LED);
      //    delay_ms(100);
      //    output_low(PORTC, LED);
    }
    
    if(ir_rx_cnt >= IR_TIME_BUF_SZ)
      {
	output_high(PORTC, LED);      	
	for(i=0;i<ir_rx_cnt-2;i++)
	  {
	    uint16_t t1,t2;
	    uint8_t out = 0;
	    t1 = ir_time_buf[i];
	    t2 = ir_time_buf[i+1];
	    
	    if ((t2 - t1 > 0) && (t2 - t1 < 24))
	      out = 0;
	    else if ((t2 - t1 >= 24) && (t2 - t1 < 48))
	      out = 1;
	    else if ((t2 - t1 >= 48) && (t2 - t1 < 100))
	      out = 0xff;
	    else
	      out = 0xfe;
	    printf("%d ", out);
	    ir_time_buf[i] = out;
	  }
	printf("\n");
	output_low(PORTC, LED);	
	ir_rx_cnt = 0;
	start = 0;	
      }
  }
}
*/
