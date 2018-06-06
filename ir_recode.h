/*
 * ATMega 328P AVR, IR re- and decoding functions
 * -----------------------------------------------
 *
 *  (C) 4/2018 Ossi Kerminen
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 */
#ifndef IR_RECODE_H
#define IR_RECODE_H

#include <stdint.h>

#if 1
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif


uint32_t ir_recode(uint32_t ir_code);
uint8_t ir_time_diff_decode(uint16_t timediff);
uint32_t ir_time_buf_decode(uint16_t *ir_time_buf, uint16_t *time_buf_len);

void ir_make_time_buf(uint32_t ir_code, uint16_t *ir_time_buf);

uint8_t ir_digit_check_and_get(uint8_t bit_cnt, uint8_t *bit_buf);


#define IR_NOP   0xfa
#define IR_END   0xfb
#define IR_REP   0xfc
#define IR_CONT  0xfd
#define IR_START 0xfe
#define IR_STOP  0xff


#define IR_FIELD_PROG_NUMBER   1
#define IR_FIELD_WEEKDAY       2
#define IR_FIELD_START_HOURS   3
#define IR_FIELD_START_MINUTES 4
#define IR_FIELD_END_HOURS     5
#define IR_FIELD_END_MINUTES   6
#define IR_FIELD_CHANNEL_HI    8
#define IR_FIELD_CHANNEL_LO    9



typedef struct ir_timer_t {
  uint8_t weekday: 3;    
  uint8_t start_h:5;
  uint8_t ch_lo: 3;
  uint8_t ch_hi: 3;      
  uint8_t start_m:6;
  uint8_t end_m:6;
  uint8_t weekly_prog: 1;
  uint8_t all_days: 1;
  uint8_t sec_week:1;
  uint8_t end_h:5;
  uint8_t prog_num: 4;
  uint8_t reserved: 4;
} ir_timer_t;

#define IR_BIT_PATTERN1 0b000111
#define IR_BIT_PATTERN2 0b101000

#endif
