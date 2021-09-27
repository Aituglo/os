#include "process.h"
#include "stdio.h"
#include "cpu.h"

struct process process_table[PROCESS_TABLE_SIZE];
int current_process_table_index = 0;

void add_process(struct process new_process){
    if(current_process_table_index < PROCESS_TABLE_SIZE){
        process_table[current_process_table_index] = new_process;
        current_process_table_index++;
    }
}

void idle(void)
{
    printf("[idle] je tente de passer la main a proc1...\n");
    //ctx_sw(..., ...);
}

void proc1(void)
{
    printf("[proc1] idle m’a donne la main\n");
    printf("[proc1] j’arrete le systeme\n");
    hlt();
}