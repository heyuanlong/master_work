#ifndef __VS_PROCESS_CYCLE_H_
#define __VS_PROCESS_CYCLE_H_


#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>

#include "vs_core.h"

#define PROCESS_NUMS 2

extern int vs_child_quit;
extern int vs_all_quit;
extern int have_child_quit;


void vs_master_process_cycle(vs_cycle_t   *cycle);						//master进程函数


#endif