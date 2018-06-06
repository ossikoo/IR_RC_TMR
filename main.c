/*
 * ATMega 328P AVR, IR RX and recode-TX modified coding
 * ----------------------------------------------------
 *
 *  (C) 4/2018 Ossi Kerminen
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 *      With this you can get rid of bad IR remote control and replace it.
 *      Turns any NEC1 remote control unit to "programmable" by first 
 *      receiving one code and sending another thru search table.
 *
 */
#include<avr/io.h>
#include<avr/wdt.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#include "generic.h"
#include "usart.h"
#include "ir_rx.h"
#include "ir_tx.h"
#include "ir_recode.h"
#include "owndelay.h"

static FILE mystdout = FDEV_SETUP_STREAM(USART_Transmit, NULL,
                                            _FDEV_SETUP_WRITE);

int main(void)
{
  uint16_t *ircode_buf = NULL;
  uint32_t codeval,new_codeval;

  uint8_t i, ircode_len;
  ircode_buf = (uint16_t*)malloc(sizeof (uint16_t)*IRCODE_BUF_SZ);
  set_output(DDRC, LED);
  set_output(DDRB,PB2);
  
  stdout = &mystdout;
  
  USART_Init();

  delay_ms(100);

  while (1) {
    codeval = ir_recv(ircode_buf, &ircode_len);
    printf("sz:%d recv_buf:",ircode_len);
    for(i=0;i<ircode_len;i++)
      printf("%d,",ircode_buf[i]);
    printf("\n");

    new_codeval = ir_recode(codeval);
    printf("0x%lx -> 0x%lx\n",codeval, new_codeval);
    
    if (ircode_len >= 2) //spurious transfer size is allmost allways one
      ir_send(new_codeval, ircode_buf);
    else
      printf("Spurious, not sending.\n");
   }
}
