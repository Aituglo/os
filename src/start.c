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

    struct process idle_process = {
        0,
        "idle",
        RUNNING
    };

    add_process(idle_process);

    struct process proc1_process = {
        1,
        "proc1",
        RUNNABLE
    };

    add_process(proc1_process);

    // *** FIN INITIALISATION PROCESSUS ***

    // *** LANCEMENT DU PREMIER PROCESSUS ***
    idle();

    sti();

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}

