#include "inttypes.h"
#ifndef _PROCESS_H_
#define _PROCESS_H_

enum STATES { RUNNING, RUNNABLE, SLEEPING, DEAD };
#define STACK_SIZE 512
#define PROCESS_TABLE_SIZE 8

struct process{
    int pid;
    char name[30];
    enum STATES state;
    uint32_t save_zone[5];
    int stack[STACK_SIZE];
    struct process *next;
    uint32_t time;
};

struct process_list{
    struct process *head;
    struct process *tail;
};


struct process *process_table[PROCESS_TABLE_SIZE];
struct process *actif;

struct process_list *runnable_list;
struct process_list *sleeping_list;
struct process_list *dead_list;

void init_process_list(void);

void remove_head(struct process_list *list);

void insert_tail(struct process_list *list, int32_t pid);

void destroy_dead(void);

void fin_processus(void);

void ordonnance(void);

int32_t add_process(char* name, void (*func));

void *ctx_sw(uint32_t *previous_context, uint32_t *new_context);

int32_t mon_pid(void);

char *mon_nom(void);

void idle(void);

void proc1(void);
void proc2(void);
void proc3(void);
void proc4(void);
void proc5(void);
void proc6(void);
void proc7(void);

#endif
