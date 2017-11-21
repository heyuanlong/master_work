#include "vs_process_cycle.h"
#include "vs_process.h"
#include "vs_net_channel.h"
#include "vs_conn.h"
#include "vs_event.h"
#include "vs_net.h"
#include "vs_busi.h"

int process_num = PROCESS_NUMS;

int vs_child_quit;
int vs_all_quit;
int have_child_quit;

void vs_start_worker_processes(vs_cycle_t *cycle);				//开启工作进程
void vs_worker_process_cycle(vs_cycle_t *cycle,void *data);		//工作进程函数
static int vs_worker_process_init(vs_cycle_t *cycle);

void vs_master_process_cycle(vs_cycle_t *cycle)
{
	
	sigset_t						set;
	int								start_nums,i;

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
	/*
	ctrl + c		kill -2    重启所有子进程
	ctrl + "\"		kill -3    关闭所有进程
	*/
	for ( ;; ) {
		sigsuspend(&set);
		if (vs_all_quit == 1) {
			exit(0);
		}
		if (vs_child_quit == 1) {
			vs_start_worker_processes(cycle);
			vs_child_quit = 0;
		}
		if (have_child_quit == 1) {
			start_nums = 0;
			for (i = 0; i < vs_last_process; i++) {
				if (vs_processes[i].pid != -1) {
					start_nums++;
				}
			}
			for (i = 0; i < process_num - start_nums; i++) {
				if (vs_spawn_process(cycle, vs_worker_process_cycle, NULL) != VS_OK) {
					vs_log_sys_error("restart vs_spawn_process fail");
				}
			}
			have_child_quit = 0;
		}
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
	int						i;

	//cpuset_setaffinity
	if(vs_event_process_init(cycle) != VS_OK){
		vs_log_sys_error("vs_event_process_init fail");
		return VS_ERROR;
	}

	vs_net_add_listen_event(cycle->tcp_listener->fd);
	vs_net_add_channel_event(vs_channel);

	for ( i = 0; i < vs_last_process; i++)
	{
		if (vs_processes[i].channel[0] != -1) {  //按理说，这里不用加条件判断。
			close(vs_processes[i].channel[0]);
			vs_processes[i].channel[0] = -1;
		}
	}

	if (ko_busi_init() != VS_OK) {
		vs_log_sys_error("ko_busi_init fail");
		return VS_ERROR;
	}

	return VS_OK;
}