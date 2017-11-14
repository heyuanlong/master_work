#include <sys/epoll.h>


#include "vs_event.h"
#include "vs_net.h"
#include "vs_process.h"




#define EPOLL_SIZE 1024

static int                  ep = -1;
static struct epoll_event  	*event_list;
static int           		nevents;


int vs_event_process_init(vs_cycle_t *cycle)
{
	vs_conn_t*				c;
	vs_event_t*				rev;

	nevents = EPOLL_SIZE;
	event_list = malloc(sizeof(struct epoll_event) * nevents );
	ep = epoll_create(nevents);
	if(ep == -1){
		printf("%s fail\n","epoll_create" );
		return VS_ERROR;
	}


	return VS_OK;
}
int vs_process_events(vs_cycle_t *cycle)
{
	int                	nums;
	int					events;
	int 				timer;
	vs_conn_t 			*c;
	vs_event_t 			*rev;
	vs_event_t 			*wev;
	int 				i;

	timer = 5000;
	nums = epoll_wait( ep, event_list, nevents, timer );
	printf("epoll_wait ok %d\n",nums );
	if( nums < 0 ){
		switch( errno ){
			case EBADF:
			case EINVAL:
				//log
				return VS_ERROR;
		}
	}
	if( nums <= 0 ){
		printf("%s\n","epoll_wait timeout" );
		return VS_OK;
	}
	//vs_log_sys_info("process:%d --%s %d\n",getpid(),"epoll_wait-----------------",nums);

	for( i = 0; i < nums; ++i ){
		c = event_list[ i ].data.u64;
		if( c->fd == -1 ){
			//ko_log_error( "event process socket -1\n" );
			continue;
		}
		events = event_list[ i ].events;
		if( events & (EPOLLERR|EPOLLHUP) ){
            //ko_log_print_debug("epoll event error, revents:%d", revents);
			//ko_conn_close( c );
			continue;
		}

		rev = c->rev;
		if (rev == NULL){
			//printf("%s\n", "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
		}else{
			//printf("%s\n", "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr---go");
			if( ( events & EPOLLIN ) && rev->active ){
				if (rev->handle){
					//printf("%s\n", "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr---go1111111111");
					rev->handle(rev);
				}
			}
		}

        wev = c->wev;
        if (wev == NULL){
			//printf("%s\n", "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
		}else{
			//printf("%s\n", "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww---go");
			if( ( events & EPOLLOUT ) && wev->active ){
				if (wev->handle){
					//printf("%s\n", "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww---go1111111111");
					wev->handle(wev);
				}
			}
		}
	}

	return VS_OK;
}

int vs_event_add( vs_event_t* ev, int type )
{
	return VS_OK;
}
int vs_event_del( vs_event_t* ev, int type )
{
	return VS_OK;
}
int vs_event_add_conn( void* conn )
{
	vs_conn_t*			  c;
	struct epoll_event	  ee;
	int					  etype;
    vs_event_t*		  rev;
	vs_event_t*		  wev;
   
	c = conn;
	rev = c->rev;
	wev = c->wev;
	rev->active = 1;
	wev->active = 1;
	rev->type = VS_EVENT_TYPE_READ;
	wev->type = VS_EVENT_TYPE_WRITE;

	etype = EPOLLIN | EPOLLOUT | EPOLLET;

	ee.events = etype;
	ee.data.u64 = c;
	printf("%s\n","epoll_ctl" );
	if (epoll_ctl( ep, EPOLL_CTL_ADD, c->fd, &ee ) < 0){
		printf("epoll_ctl fail: %s\n",strerror(errno) );
		return VS_ERROR;
	}

	return VS_OK;
}
int vs_event_del_conn( void* conn )
{
	vs_conn_t*				c;
    vs_event_t				*rev, *wev;

	c = conn;
    rev = c->rev;
	wev = c->wev;
    rev->active = 0;
	wev->active = 0;    

	if (epoll_ctl( ep, EPOLL_CTL_DEL,  c->fd,NULL ) < 0){
		printf("epoll_ctl EPOLL_CTL_DEL fail: %s\n",strerror(errno) );
		return VS_ERROR;
	}
	return VS_OK;
}