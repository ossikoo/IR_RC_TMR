/*
 * ATMega 328P AVR, IR TX module
 * -----------------------------------------------
 *
 *  (C) 4/2018 Ossi Kerminen
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 */
#ifndef IR_TX_H
#define IR_TX_H

#include "ir_generic.h"

#if 0
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

void init_ir_tx(void);
/* Function: ir_send
       Send IR 
   Params:
       ir_code     uint32_t NEC-1 IR code
       ir_time_buf ptr to IR code buffer
        
*/
void ir_send(uint32_t ir_code, uint16_t *ir_time_buf);

void send_ir_start(void);
void send_ir_digit(uint8_t digit);
void send_ir_stop(void);
void send_ir_tx(uint16_t *ir_time_buf);

#define PWM_OFF set_input(DDRB, IR_TX);
#define PWM_ON  set_output(DDRB, IR_TX);

#endif
