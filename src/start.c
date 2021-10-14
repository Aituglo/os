#include <cpu.h>
#include <process.h>
#include <interruptions.h>
#include <screen.h>
#include <stdio.h>
#include <inttypes.h>

const uint32_t QUARTZ = 0x1234DD;
const uint32_t CLOCKFREQ = 50;

void kernel_start(void)
{
    clear_screen();

    // *** DEBUT GESTION HORLOGE ***

    // Reglage de l'horloge
    outb(0x34, 0x43);
    outb((QUARTZ / CLOCKFREQ) % 256, 0x40);
    outb((QUARTZ / CLOCKFREQ) / 256, 0x40);
   
    init_traitant_IT(32, traitant_IT_32);

    masque_IRQ(0, 0);

    // *** FIN GESTION HORLOGE **

    // *** INITIALISATION PROCESSUS ***

    init_process_list();

    add_process("idle", idle);
    actif = process_table[0];

    add_process("proc1", proc1);
    add_process("proc2", proc2);
    add_process("proc3", proc3);
    add_process("proc4", proc4);
    //add_process("proc5", proc5);
    //add_process("proc6", proc6);
    //add_process("proc7", proc7);

    // *** FIN INITIALISATION PROCESSUS ***

    // *** LANCEMENT DU PREMIER PROCESSUS ***
    idle();

}

