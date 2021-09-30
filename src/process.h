#include "inttypes.h"
#ifndef _PROCESS_H_
#define _PROCESS_H_

enum STATES { RUNNING, RUNNABLE };
#define STACK_SIZE 512
#define PROCESS_TABLE_SIZE 5

struct process{
    int pid;
    char name[30];
    enum STATES state;
    uint32_t save_zone[5];
    int stack[STACK_SIZE];
};

void add_process(struct process new_process);

void idle(void);

void proc1(void);

#endif
