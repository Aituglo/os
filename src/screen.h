#include "inttypes.h"
#ifndef __SCREEN_H__
#define __SCREEN_H__

uint16_t *ptr_mem(uint32_t line, uint32_t col);

void write_char(uint32_t line, uint32_t col, char c);

void clear_screen(void);

void place_cursor(uint32_t line, uint32_t col);

void process_char(char c);

void scroll(void);

void console_putbytes(const char *s, int len);

#endif
