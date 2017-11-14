#ifndef __VS_PROCESS_CYCLE_H_
#define __VS_PROCESS_CYCLE_H_


#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>

#include "vs_core.h"

extern int vs_reap;
extern int vs_terminate;
extern int vs_quit;



void vs_master_process_cycle(vs_cycle_t   *cycle);						//master进程函数


#endif