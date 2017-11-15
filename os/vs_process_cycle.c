#include "vs_process_cycle.h"
#include "vs_process.h"
#include "vs_net_channel.h"
#include "vs_conn.h"
#include "vs_event.h"
#include "vs_net.h"


int process_num = 1;
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
    	vs_log_sys_error("sigprocmask fail\n");
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
		printf("%s fail\n", "vs_worker_process_init");
		return;
	}
	for ( ; ; )
	{
		//printf("%s\n","vs_process_events start" );
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
	//取消信号阻塞
	if(vs_event_process_init(cycle) != VS_OK){
		printf("%s fail\n", "vs_event_process_init");
		return VS_ERROR;
	}

	c = vs_conn_get(cycle->tcp_listener->fd);
	rev = c->rev;
	rev->handle = vs_net_accept_handle;
	vs_event_add_conn(c, VS_EVENT_TYPE_READ);

	vs_net_add_channel_event(vs_channel);

	return VS_OK;
}