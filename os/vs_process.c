#include "vs_process.h"
#include "vs_core.h"
#include "vs_socket.h"

#include "vs_process_cycle.h"
#include "vs_net_channel.h"

 int             vs_process_slot;
 int             vs_last_process;
 vs_process_t    vs_processes[VS_MAX_PROCESSES];
 int             vs_channel;


int vs_spawn_process(vs_cycle_t *cycle,vs_spawn_proc_pt proc, void *data){
    pid_t   pid;
    int     i;
    for (i = 0; i < vs_last_process; i++) {
        if (vs_processes[i].pid == -1) {
            break;
        }
    }
    
    //socketpair        
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, vs_processes[i].channel) == -1){
        vs_log_sys_error("socketpair fail:%s",strerror(errno));
        return VS_ERROR;
    }
    if (vs_nonblocking(vs_processes[i].channel[0]) == -1){
        vs_log_sys_error("vs_nonblocking fail:%s",strerror(errno));
        return VS_ERROR;
    }
    if (vs_nonblocking(vs_processes[i].channel[1]) == -1){
        vs_log_sys_error("vs_nonblocking fail:%s",strerror(errno));
        return VS_ERROR;
    }
    vs_process_slot = i;
    vs_channel = vs_processes[i].channel[1];
    
    pid = fork();
    switch (pid) {
    case -1:
        vs_log_sys_error("fork fail:%s",strerror(errno));
        return VS_ERROR;
    case 0:
		if (i == vs_last_process) {
			vs_last_process++;
		}
        proc(cycle, data );
        break;
    default:
        break;
    }
    
    close(vs_processes[i].channel[1]);
	vs_processes[i].channel[1] = -1;

    vs_processes[i].index = i;
    vs_processes[i].pid = pid;
    vs_processes[i].status = 0;
	vs_processes[i].exited = 0;
    vs_processes[i].proc = proc;
    vs_processes[i].data = data;

    

    if (i == vs_last_process) {
        vs_last_process++;
    }
    return VS_OK;

}






//----------------------------------------------------------------------------
