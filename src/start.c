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

    // Traitant pour le clavier
    init_traitant_IT(33, traitant_IT_9);
   
    // Traitant pour le temps
    init_traitant_IT(32, traitant_IT_32);


    // IRQ0 pour le temps
    masque_IRQ(0, 0);

    // IRQ1 pour le clavier
    masque_IRQ(1, 0);

    // *** FIN GESTION HORLOGE **

    // *** INITIALISATION PROCESSUS ***

    init_process_list();

    add_process("idle", idle);
    actif = process_table[0];

    print_startup();

    print_prompt();

    // *** FIN INITIALISATION PROCESSUS ***

    // *** LANCEMENT DU PREMIER PROCESSUS ***
    idle();

}

