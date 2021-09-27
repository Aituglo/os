#include "stdio.h"
#include "screen.h"
#include "segment.h"
#include "interruptions.h"
#include "cpu.h"

// in ms
int UPTIME = 0;

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

