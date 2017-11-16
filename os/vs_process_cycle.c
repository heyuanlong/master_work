#include "vs_process_cycle.h"
#include "vs_process.h"
#include "vs_net_channel.h"
#include "vs_conn.h"
#include "vs_event.h"
#include "vs_net.h"


int process_num = PROCESS_NUMS;
int vs_reap;
int vs_terminate;
int vs_quit;


void vs_start_worker_processes(vs_cycle_t *cycle);				//开启工作进程
void vs_worker_process_cycle(vs_cycle_t *cycle,void *data);		//工作进程函数
static int vs_worker_process_init(vs_cycle_t *cycle);

void vs_master_process_cycle(vs_cycle_t *cycle)
{
	
	sigset_t						set;

	sigemptyset(&set);
    sigaddset(&set,SIGHUP);
    sigaddset(&set,SIGTERM);
    sigaddset(&set,SIGQUIT);
    sigaddset(&set,SIGALRM);
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGIO);
    sigaddset(&set,SIGCHLD);
    sigaddset(&set,SIGSYS);
    sigaddset(&set,SIGPIPE);	//屏蔽信号

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
    	vs_log_sys_error("sigprocmask fail");
        return;
    }
	sigemptyset(&set);

	vs_start_worker_processes(cycle);			//fork工作进程

	for ( ;; ) {
		sigsuspend(&set);
	}
}


void vs_start_worker_processes(vs_cycle_t *cycle)
{
	int i = 0;
	for (i = 0; i < process_num; i++) {
		if(vs_spawn_process(cycle,vs_worker_process_cycle,NULL) != VS_OK){
			vs_log_sys_error("vs_spawn_process fail");
			return;
		}
	}
}

void vs_worker_process_cycle(vs_cycle_t *cycle,void *data)
{
	
	if(vs_worker_process_init(cycle) != VS_OK){
		vs_log_sys_error("vs_worker_process_init fail");
		return;
	}
	for ( ; ; )
	{
		vs_time_update();
		vs_event_timer_expire();
		vs_process_events(cycle);
	}

	exit(0);
}
static int vs_worker_process_init(vs_cycle_t *cycle)
{
	vs_conn_t          		*c;
	vs_event_t 				*rev;

	//cpuset_setaffinity
	if(vs_event_process_init(cycle) != VS_OK){
		vs_log_sys_error("vs_event_process_init fail");
		return VS_ERROR;
	}

	vs_net_add_listen_event(cycle->tcp_listener->fd);
	vs_net_add_channel_event(vs_channel);

	return VS_OK;
}