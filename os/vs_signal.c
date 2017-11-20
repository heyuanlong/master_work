#include "vs_signal.h"

#include "vs_process.h"
#include "vs_process_cycle.h"
#include <unistd.h>


static void vs_process_get_status();
void vs_signal_handler(int signo);


typedef struct {
    int     signo;
    char   *signame;
    char   *name;
    void  (*handler)(int signo);
} vs_signal_t;

vs_signal_t  signals[] = {
    { SIGHUP,   "SIGHUP", "reload", 	vs_signal_handler },
    { SIGTERM,  "SIGTERM", "stop", 	vs_signal_handler },
    { SIGQUIT,  "SIGQUIT", "quit", 	vs_signal_handler },
    { SIGALRM,  "SIGALRM", "", 		vs_signal_handler },
    { SIGINT,   "SIGINT", "", 		vs_signal_handler },
    { SIGIO,    "SIGIO", "", 			vs_signal_handler },
    { SIGCHLD,  "SIGCHLD", "", 		vs_signal_handler },
    { SIGSYS,   "SIGSYS, SIG_IGN", "", 	SIG_IGN },
    { SIGPIPE,  "SIGPIPE, SIG_IGN", "", 	SIG_IGN },
    { 0, NULL,  "", NULL }
};

int vs_init_signals( )
{
    vs_signal_t      *sig;
    struct sigaction   sa;
	
    for (sig = signals; sig->signo != 0; sig++) {
        memset(&sa, 0, sizeof(struct sigaction));
        sa.sa_handler = sig->handler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(sig->signo, &sa, NULL) == -1) {
            return VS_ERROR;
        }
    }

    return VS_OK;
}

void vs_signal_handler(int signo)
{
	vs_signal_t      *sig;
	for (sig = signals; sig->signo != 0; sig++) {
        if (sig->signo == signo) {
            break;
        }
    }
    switch (signo) {
        case SIGHUP:
			//vs_child_reap = 1;
            break;
        case SIGTERM:
           // vs_terminate = 1;            
            break;
        case SIGQUIT:		//ctrl +"\"
			vs_log_sys_info("GET SIGQUIT");
			vs_all_quit = 1;
			vs_net_tell_child_quit();
			sleep(1);	//这句要加上，否则一些子进程的channel socket收到EPOLLERR|EPOLLHUP
            break;
        case SIGINT:		//ctrl + c
			vs_log_sys_info("GET SIGINT");
			vs_child_quit = 1;
			vs_net_tell_child_quit();
            break;
		case SIGCHLD:
			vs_log_sys_info("GET SIGCHLD");
			have_child_quit = 1;
			vs_process_get_status();
			break;
    }

    return;
}

static void vs_process_get_status()
{
    int         status;             //记载进程终止原因。
    pid_t       pid;
    int         err;
    int         i;
    unsigned int    one;

    one = 0;

    for( ; ;){
        pid = waitpid(-1,&status,WNOHANG);
        if(pid == 0){
            return ;
        }
        if (pid == -1){
            err = errno;
            if (err == EINTR) {
                continue;
            }
            if (err == ECHILD && one) {
                return;
            }
            /*
             * Solaris always calls the signal handler for each exited process
             * despite waitpid() may be already called for this process.
             *
             * When several processes exit at the same time FreeBSD may
             * erroneously call the signal handler for exited process
             * despite waitpid() may be already called for this process.
             */
            if (err == ECHILD) {
                return;
            }
            return;
        }

        one = 1;
        for (i = 0; i < vs_last_process; i++) {
            if (vs_processes[i].pid == pid) {
				vs_processes[i].pid = -1;
                vs_processes[i].status = status;
                vs_processes[i].exited = 1;
                break;
            }
        }
    }
    return ;
}





//----------------------------------------------------------------------------