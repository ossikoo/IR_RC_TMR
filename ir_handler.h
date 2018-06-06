#ifndef IR_HANDLER_H
#define IR_HANDLER_H

#include <stdint.h>

#if 1
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

#define IR_HANDLER_MIN_SZ 2
#define IR_HANDLER_BUTTON_CODE_SZ 24

#define IR_HANDLER_TIMER_BIT_SZ 240
#define IR_HANDLER_TIMER_BYTE_SZ IR_HANDLER_TIMER_BIT_SZ / 8

void handle_ir(uint16_t bit_size, uint8_t *bit_buf);
void handle_ir_button(uint8_t *bit_buf);
void handle_ir_timer(uint8_t *bit_buf);

#define IR_BUTTON1 0
#define IR_BUTTON_FUNCTION 32
#define IR_BUTTON_ERROR 255
#endif
