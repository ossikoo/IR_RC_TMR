#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include "generic.h"
#include "ir_recode.h"
#include "rtc.h"

rtc_t cur_time;
ir_timer_t *ir_tmr = NULL;

void rtc_time_tick(void)
{
  cur_time.s++;
  if(cur_time.s >= 60)
    {
      cur_time.s -= 60;
      cur_time.m++;
    }
  if(cur_time.m >= 60)
    {
      cur_time.m -=60;
      cur_time.h++;
    }
  if(cur_time.h >= 24)
    {
      cur_time.h -= 24;
      cur_time.d++;
    }
  if(cur_time.d >= 14)
    {
      cur_time.d -= 14;
    }
  dbg_printf(" %d %d:%d:%d\n", cur_time.d, cur_time.h, cur_time.m, cur_time.s);
}

uint8_t rtc_time_set(uint8_t d, uint8_t h, uint8_t m, uint8_t s)
{
  cur_time.d = d;
  cur_time.h = h;
  cur_time.m = m;
  cur_time.s = s;
  dbg_printf(" rtc_time_set: %d %2.2d:%2.2d:%2.2d\n", cur_time.d, cur_time.h, cur_time.m, cur_time.s);
  return 0;
}

rtc_t *rtc_time_get(void)
{
  rtc_t *rtc = (rtc_t*)malloc(sizeof(rtc_t));
  rtc->s = cur_time.s;
  rtc->m = cur_time.m;
  rtc->h = cur_time.h;
  rtc->d = cur_time.d;
  return rtc;
}

void rtc_activate_timer(ir_timer_t *ir_timer)
{

  if(ir_tmr != NULL)
    free(ir_tmr);

  ir_tmr = ir_timer;
}

uint8_t rtc_timer_on(void)
{
  dbg_printf("%d: %d:%d %d:%d\n", ir_tmr->weekday, ir_tmr->start_h, ir_tmr->start_m, ir_tmr->end_h, ir_tmr->end_m);
  if(cur_time.d != ir_tmr->weekday)
    return 0;
    
  if(cur_time.h < ir_tmr->start_h ||
     cur_time.h > ir_tmr->end_h)
    return 0;
  
    
  if(cur_time.m < ir_tmr->start_m ||
     cur_time.m >= ir_tmr->end_m) /////////////////////////////////////////////////////////////////////
    return 0;
  

  dbg_printf("timer on!");
  return 1;
}
