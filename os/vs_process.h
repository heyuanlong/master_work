#ifndef __VS_PROCESS_H_
#define __VS_PROCESS_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>         
#include <sys/socket.h>

#include "vs_core.h"


#define VS_MAX_PROCESSES         64


typedef void (*vs_spawn_proc_pt) (vs_cycle_t *cycle,void *data);
typedef struct {
	int 				index;
    pid_t           	pid;
    int                 status;
    int        			channel[2];

    vs_spawn_proc_pt   proc;
    void               *data;

    int					exited;
} vs_process_t;



extern int   vs_channel;
extern int   vs_last_process;
extern vs_process_t    vs_processes[VS_MAX_PROCESSES];

int 	vs_spawn_process(vs_cycle_t *cycle,vs_spawn_proc_pt proc, void *data);		//fork进程


#endif