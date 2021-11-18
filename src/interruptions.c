#include "stdio.h"
#include "screen.h"
#include "segment.h"
#include "process.h"
#include "interruptions.h"
#include "cpu.h"
#include "keyboard_map.h"

#define COMMAND_BUFFER_SIZE 100
#define PROMPT_LENGTH 6

char cmd_buffer[COMMAND_BUFFER_SIZE];
int cmd_len = 0;

// in ms
int UPTIME = 0;

int get_uptime(){
    return UPTIME / 1000;
}

void tic_PIT(void){
    outb(0x20, 0x20);

    UPTIME += 20;

    int ms = UPTIME;

    int seconds = ms / 1000;
    ms %= 1000;

    int minutes = seconds / 60;
    seconds %= 60;

    int hours = minutes / 60;
    minutes %= 60;

    char current_time[9];
    sprintf(current_time,"%02d:%02d:%02d", hours, minutes, seconds);

    write_top_right(current_time);

    ordonnance();
}

bool streq(char* string1, int str1len, char* string2, int str2len) {
	if (str1len != str2len) return false;
	for (int i = 0; i < str1len; i++) {
		if (string1[i] != string2[i]) return false;
	}
	return true;
}

void keyboard_PIT(void){
    outb(0x20, 0x20);
    
    unsigned char status = inb(0x64);


	if (status & 0x1) {
		int keycode = inb(0x60);

		if (keycode < 0 || keycode >= 128) return;

		if (keycode == 28) {
			// ENTER : Newline
			current_line++;
			current_col = 0;
            if (streq(cmd_buffer, cmd_len, "ls", 2)) {
				printf("Filesystem not yet implemented.\n");
			} else if (streq(cmd_buffer, cmd_len, "clear", 5)) {
				clear_screen();
                current_line = 0;
                current_col = 0;
                place_cursor(current_line, current_col);
                print_startup();
			} else if (cmd_len < 1) {
				// do nothing
			} else {
				printf("Command not found: ");
				printf("%s\n", cmd_buffer);
			}
			cmd_len = 0;
            place_cursor(current_line, current_col);
            print_prompt();
		} else if (keycode == 14) {
			// BACKSPACE: Move back one unless on prompt
            if (current_col > PROMPT_LENGTH) {
				write_char(current_line, --current_col, (char)0);
                place_cursor(current_line, current_col);
                cmd_buffer[cmd_len] = keyboard_map[0];
			}
			
		} else {
			
            if (cmd_len >= COMMAND_BUFFER_SIZE) return;

			cmd_buffer[cmd_len++] = keyboard_map[keycode];

			printf("%c", keyboard_map[keycode]);

		}
	}
}


void init_traitant_IT(int32_t num_IT, void (*traitant)(void)){
    
    uint32_t *addr = (uint32_t *)(0x1000 + num_IT*2*4);

    uint16_t low = (uint32_t)traitant & 0xFFFF;
    uint16_t high = ((uint32_t)traitant >> 16) & 0xFFFF;

    uint32_t first_part = ((uint16_t)KERNEL_CS << 16) | low;
    uint32_t last_part = ((uint16_t)high << 16) | 0x8E00;

    *addr = first_part;
    addr += 1;
    *addr = last_part;
}

void masque_IRQ(uint32_t num_IRQ, bool masque){
    uint8_t current = inb(0x21);
    if(masque){
        current |= 1 << num_IRQ;
    }else{
        current &= ~(1 << num_IRQ);
    }  
    outb(current, 0x21);
}

