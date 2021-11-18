#include "screen.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "cpu.h"

uint32_t current_line = 0;
uint32_t current_col = 0;

uint16_t *ptr_mem(uint32_t line, uint32_t col){
    return (uint16_t *)(0xB8000 + 2*(line*80 + col));
}

void write_char(uint32_t line, uint32_t col, char c){
    uint16_t *addr = ptr_mem(line, col);

    uint8_t default_color = 15;
    uint8_t current_char = (uint8_t)c;

    uint16_t final = ((uint16_t)default_color << 8) | current_char;

    *addr = final;
}

void clear_screen(void){
    for(int line = 0; line < 25; line++){
        for(int col = 0; col < 80; col++){
            uint16_t *addr = ptr_mem(line, col);

            *addr = 0b0000111100100000;
        }
    }

    place_cursor(0, 0);
}

void place_cursor(uint32_t line, uint32_t col){
    uint16_t pos = col + line*80;

    uint8_t low = pos & 0xFF;
    uint8_t high = (pos >> 8) & 0xFF;

    outb(0x0F, 0x3D4);

    outb(low, 0x3D5);

    outb(0x0E, 0x3D4);

    outb(high, 0x3D5);
}

void write_top_right(char *s){
    size_t size = 8;

    uint32_t line = 0;
    uint32_t first_col = 80 - size;

    for(int i = 0; i < size; i++){
        write_char(line, first_col + i, s[i]);
    }
}

void process_char(char c){
    uint8_t ascii = (uint8_t)c;

    if(ascii >= 32 && ascii <= 126){
        write_char(current_line, current_col, c);
    }

    // Compute the new current position
    if(current_col < 79){
        current_col++;
    }else if (current_col == 79)
    {
        current_col = 0;
        current_line++;
    }

    switch(ascii){
        case 8:
            if(current_col != 0){
                current_col--;
            }
            break;
        case 9:
            if(current_col + 8 < 79){
                current_col += 8;
            }else if(current_col + 8 >= 79){
                current_col = 79;
            }
            break;
        case 10:
            current_col = 0;
            current_line++;
            break;
        case 12:
            clear_screen();
            current_line = 0;
            current_col = 0;
            break;
        case 13:
            current_col = 0;
            break;
    }

    if(current_line == 24){
        scroll();
    }
    
    place_cursor(current_line, current_col);
}

void print_prompt(void){
    printf("shell>");
}

void print_startup(void){
    printf("*** Welcome to CassimOS 1.0 ***\n\n");
}

void scroll(void){
    uint16_t *source = ptr_mem(1, 0);
    uint16_t *destination = ptr_mem(0, 0);

    size_t size = 80 * 24 * 2;

    memmove(destination, source, size);

    current_line--;

    place_cursor(current_line, current_col);
}

void console_putbytes(const char *s, int len){
    for(int i = 0; i < len; i++){
        process_char(s[i]);
    }
}