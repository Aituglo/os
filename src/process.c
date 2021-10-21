#include "process.h"
#include "stdio.h"
#include <string.h>
#include "cpu.h"
#include "interruptions.h"
#include <tinyalloc.h>
#include <stddef.h>

int current_process_table_index = 0;

void init_process_list(void){
    runnable_list = malloc(sizeof(struct process_list));
    sleeping_list = malloc(sizeof(struct process_list));
    dead_list = malloc(sizeof(struct process_list));
}

int32_t add_process(char* name, void (*func)){
    if(current_process_table_index < PROCESS_TABLE_SIZE){

        process_table[current_process_table_index] = malloc(sizeof (struct process));
        struct process *new = process_table[current_process_table_index];
        new->pid = current_process_table_index;
        strcpy(new->name, name);
        new->next = NULL;

        if((new->pid) > 0){
            new->state = RUNNABLE;
            
            // On met au sommet de la pile notre code de la fonction
            new->stack[STACK_SIZE - 2] = (uint32_t) func;
            // %esp pour les registers pointe vers le sommet de la pile
            new->save_zone[1] = (uint32_t) (&(new->stack[STACK_SIZE - 2]));
      
            new->stack[STACK_SIZE - 1] = (uint32_t) fin_processus;

            process_table[current_process_table_index - 1]->next = new;

            insert_tail(runnable_list, new->pid);
            
        }else{
            new->state = RUNNING;
        }


        current_process_table_index++;
        return current_process_table_index - 1;
    }else{
        return -1;
    }
}

void remove_head(struct process_list *list){
    struct process *head = list->head;

    if(list->head == list->tail){
        // Il y a plus qu'un process
        list->head = NULL;
        list->tail = NULL;
        
    }else{
        list->head = head->next;
    }

    // On met a jour l'état
    if(list == runnable_list){
        head->state = RUNNING;
    }else{
        head->state = RUNNABLE;
    }
}

void insert_tail(struct process_list *list, int32_t pid){

    struct process *new = process_table[pid];

    if(list->head == NULL){
        list->head = new;
    }

    if(list->tail != NULL){
        list->tail->next = new;
    }

    list->tail = new;

    new->next = NULL;
    if(list == runnable_list){
        new->state = RUNNABLE;
    }
}

void ordonnance(void){

    if(dead_list->head != NULL){
        destroy_dead();
    }

    uint32_t time = get_uptime();
    if(sleeping_list->head != NULL){
        struct process *sleeping_process = sleeping_list->head;
        if(time >= (sleeping_process->time)){
            printf("[Reveil] processus %s pid = %i\n", sleeping_process->name, sleeping_process->pid);
            remove_head(sleeping_list);
            insert_tail(runnable_list, sleeping_process->pid);
        }
    }


    struct process *last = actif;

    if(runnable_list->head == NULL){
        // On boucle sur le seul proc
        ctx_sw(last->save_zone, actif->save_zone);
    }else{
        actif = runnable_list->head;
        remove_head(runnable_list);
    }

    if(last->state != SLEEPING){
        insert_tail(runnable_list, last->pid);
    }
    ctx_sw(last->save_zone, actif->save_zone);


}

void insert_sleep(){
    struct process *current = sleeping_list->head;

    if(current == NULL){
        sleeping_list->head = actif;
        sleeping_list->tail = actif;
        actif->next = NULL;
    }else{
        struct process *last = NULL;

        while(current->time <= (actif->time)){
            last = current;
            if(current == sleeping_list->tail){
                current = NULL;
                sleeping_list->tail = actif;
                
                break;
            }
            current = current->next;
        }

        // Cas tete de liste
        if(last == NULL){
            sleeping_list->head = actif;
            actif->next = current;
        }else{
            last->next = actif;
            actif->next = current;
        }
    }
}

void sleep(uint32_t time){
    printf("[Dodo] processus %s pid = %i\n", mon_nom(), mon_pid());
    uint32_t new_time = get_uptime() + time;
    actif->time = new_time;
    actif->state = SLEEPING;

    insert_sleep();

    ordonnance();
}

void fin_processus(void){
    actif->state = DEAD;
    insert_tail(dead_list, actif->pid);

    struct process *last = actif;
    actif = runnable_list->head;
    remove_head(runnable_list);

    ctx_sw(last->save_zone, actif->save_zone);
}

void destroy_dead(void){
    
    int32_t pid = dead_list->head->pid; 
    printf("[Mort] processus pid = %i\n", pid);

    free(process_table[pid]);

    process_table[pid] = NULL;

    current_process_table_index--;

    remove_head(dead_list);

}

int32_t nbr_secondes(void){
    return actif->time;
}

int32_t mon_pid(void){
    return actif->pid;
}

char *mon_nom(void){
    return actif->name;
}


void idle(void)
{
    
    for (;;) {
        //printf("[%s] pid = %i\n", mon_nom(), mon_pid());
        //printf("%u", get_uptime());
        sti();
        hlt();
        cli();
    }
}

void proc1(void)
{
    for (uint32_t i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
        mon_nom(), mon_pid());
        sleep(2);
    }
}

void proc2(void)
{
    for (uint32_t i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
        mon_nom(), mon_pid());
        sleep(3);
    }
}

void proc3(void)
{
    for (uint32_t i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
        mon_nom(), mon_pid());
        sleep(4);
    }
}

void proc4(void)
{
    for (uint32_t i = 0; i < 2; i++) {
        printf("[temps = %u] processus %s pid = %i\n", nbr_secondes(),
        mon_nom(), mon_pid());
        sleep(5);
    }
}
