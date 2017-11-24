#ifndef __VS_INIT_CYCLE_H_
#define __VS_INIT_CYCLE_H_


#include <stdlib.h>
#include <unistd.h>


#include "vs_core.h"

vs_cycle_t *	vs_cycle_create();
int				vs_init_cycle(vs_cycle_t *cycle);


#endif