/*
 * ATMega 328P AVR, IR generic definitions
 * -----------------------------------------------
 *
 *  (C) 4/2018 Ossi Kerminen
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 */
#ifndef IR_GENERIC_H
#define IR_GENERIC_H

//Port pins
#define IR_RX PB0
#define IR_TX PB2

//Minimum buffer size is 34 + 1 for start counter value.
#define IRCODE_BUF_SZ 200
//Received IR byte-triplets
#define IR_TRIPLET_BUF_SZ 32
//IR Pulse lengths(in 1/10 of a microseconds)
#define IR_SHORT 56
#define IR_LONG  153
#define IR_MAX_WAIT_TIME 650
#endif
