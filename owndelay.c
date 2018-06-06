#include "generic.h"
#include "owndelay.h"


void delay_ms(uint8_t ms)
{
  uint16_t delay_count = F_CPU / 19205;
  volatile uint16_t i;

  while (ms != 0) {
    for (i=0; i != delay_count; i++);
    ms--;
  }
}

void delay_10us(uint16_t us10)
{
  uint32_t fcpu = F_CPU; //F_CPU = 14.74MHz
  uint16_t delay_count;
  volatile uint16_t i;
  uint8_t ms;

  ms = us10 / 100;
  delay_count = fcpu * (us10 % 100) / 1988220; //fcpu=14.74MHz

  if(ms) {
    delay_ms(ms);
  
    if (delay_count >= ms*10)
      delay_count -= ms*10;
    else 
      return;
  } 

  //  while (loops) {
    for (i=0; i != delay_count; i++);
    //    loops--;
    //  }
}
