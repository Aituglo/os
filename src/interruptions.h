#include "inttypes.h"
#include "stdbool.h"
#ifndef __INTERRUPTION_H__
#define __INTERRUPTION_H__

int get_uptime(void);

void tic_PIT(void);

void traitant_IT_32(void);

void init_traitant_IT(int32_t num_IT, void (*traitant)(void));

void masque_IRQ(uint32_t num_IRQ, bool masque);

#endif
