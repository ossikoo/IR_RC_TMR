#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "generic.h"
#include "ir_recode.h"
#include "rtc.h"
#include "ir_handler.h"

void handle_ir_button(uint8_t *bit_buf)
{
  uint8_t button_code;
  static uint8_t func_state;
  button_code = ir_digit_check_and_get(IR_HANDLER_BUTTON_CODE_SZ, bit_buf);
  
  switch(button_code)
    {
    case IR_BUTTON1:
      dbg_printf("Button IR_BUTTON1 handled.\n");
      break;
    case IR_BUTTON_FUNCTION:
      {
	if(func_state)
	  func_state = 0;
	else
	  func_state = 1;
	
	if(func_state)
	  LED_ON
	else
	  LED_OFF
      }
      break;
    case IR_BUTTON_ERROR:
      dbg_printf("Not accepted button code.\n");
      break;
    default:
      dbg_printf("Unhandled button code %d.\n", button_code);
    }
}

void handle_ir_timer(uint8_t *bit_buf)
{
  uint8_t i=0, j=0;
  ir_timer_t *ir_timer;

  dbg_printf("handle_ir_timer:\n");
  ir_timer = (ir_timer_t*)calloc(1, sizeof(ir_timer_t));
  
  for(i=0;i<IR_HANDLER_TIMER_BYTE_SZ;i+=3)
    {
      uint8_t ir_field;
      ir_field = ir_digit_check_and_get(IR_HANDLER_BUTTON_CODE_SZ, &bit_buf[i]);
      dbg_printf("%d: %x\n", j, ir_field);     
      switch(j) {
      case IR_FIELD_START_HOURS:
	if(ir_field >= 16)
	  ir_field -=4;
	ir_timer->start_h = ir_field;
	break;
      case IR_FIELD_START_MINUTES:
	ir_timer->start_m = ir_field;
	break;
      case IR_FIELD_END_HOURS:
	if(ir_field >= 16)
	  ir_field -=4;
	ir_timer->end_h = ir_field;
	break;
      case IR_FIELD_END_MINUTES:
	ir_timer->end_m = ir_field;
	break;
      case IR_FIELD_CHANNEL_HI:
	ir_timer->ch_hi = (ir_field & 0x03);
	break;
      case IR_FIELD_CHANNEL_LO:
	{
	  uint8_t ir_temp;
	  ir_temp = ir_timer->ch_hi * 10;
	  ir_temp += ir_field & 0x0f;
	  ir_timer->ch_lo = ir_temp & 0x07;
	  ir_timer->ch_hi = ((ir_temp >> 3) & 0x07);
	}
	break;
      case IR_FIELD_PROG_NUMBER:
	ir_timer->prog_num = ir_field & 0x07;
	break;
      case IR_FIELD_WEEKDAY:
	ir_timer->weekday = ir_field & 0x07;
	ir_timer->sec_week= (ir_field & 0x08) >> 3;
	ir_timer->weekly_prog = (ir_field & 0x20) >> 5;
	if(ir_timer->sec_week && ir_timer->weekly_prog)
	  ir_timer->all_days = 1;
	break;
      default:
	break;
      }
      j++;
    }

  dbg_printf("PROG_NUM: %d\n", ir_timer->prog_num);
  dbg_printf("DAY: %d\n", ir_timer->weekday);
  dbg_printf("START: %d:%2.2d ", ir_timer->start_h, ir_timer->start_m);
  dbg_printf("END: %d:%2.2d ", ir_timer->end_h, ir_timer->end_m);
  dbg_printf("CH: (%d:%d)\n", ir_timer->ch_hi, ir_timer->ch_lo);

  //defunc:
  dbg_printf("W: %d 2nd: %d ALLD: %d\n", ir_timer->weekly_prog, ir_timer->sec_week, ir_timer->all_days);

  if(ir_timer->start_h == ir_timer->end_h &&
     ir_timer->start_m == ir_timer->end_m)
    {
      rtc_time_set(ir_timer->weekday, ir_timer->start_h, ir_timer->start_m, 0);
    }
  else
    {
      rtc_activate_timer(ir_timer);
    }
}


void handle_ir(uint16_t bit_size, uint8_t *bit_buf)
{
  if(bit_size < IR_HANDLER_MIN_SZ)
    {
      dbg_printf("Spurious transfer.\n");
      return;
    }

  dbg_printf("handle_ir: bit_size=%d\n", bit_size);

  //NOTE: Handling order matters!

  if(bit_size == IR_HANDLER_TIMER_BIT_SZ)
    {
      handle_ir_timer(bit_buf);
      return;
    }

  if(bit_size % IR_HANDLER_BUTTON_CODE_SZ == 0)
    {
      handle_ir_button(bit_buf);
      return;
    }

}
