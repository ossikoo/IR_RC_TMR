/*
 * ATMega 328P AVR, IR RX module
 * -----------------------------------------------
 *
 *  (C) 4/2018 Ossi Kerminen
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 */
#ifndef IR_RX_H
#define IR_RX_H

#if 1
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

#include "ir_generic.h"

void init_ir_rx(void);
/* Function: ir_recv
       Receive IR 
    Params:
       ir_time_buf ptr to received IR code buffer
       len         ir_time_buf length
*/
uint32_t ir_recv(uint16_t *ir_time_buf, uint8_t *len);

  
#endif
