#include "inttypes.h"
#ifndef __SCREEN_H__
#define __SCREEN_H__

extern uint32_t current_line;
extern uint32_t current_col;

uint16_t *ptr_mem(uint32_t line, uint32_t col);

void print_prompt(void);

void print_startup(void);

void write_char(uint32_t line, uint32_t col, char c);

void clear_screen(void);

void place_cursor(uint32_t line, uint32_t col);

void process_char(char c);

void scroll(void);

void write_top_right(char *s);

void console_putbytes(const char *s, int len);

#endif
