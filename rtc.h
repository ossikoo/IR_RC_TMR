#ifndef RTC_H
#define RTC_H

#if 1
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

typedef struct rtc_t {
  uint8_t d;
  uint8_t h;
  uint8_t m;
  uint8_t s;
} rtc_t;

void rtc_time_tick(void);
uint8_t rtc_time_set(uint8_t d, uint8_t h, uint8_t m, uint8_t s);
rtc_t *rtc_time_get(void);
void rtc_activate_timer(ir_timer_t *ir_timer);
uint8_t rtc_timer_on(void);


#endif
