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
	event_list = vs_palloc(cycle->pool,sizeof(struct epoll_event) * nevents);
	ep = epoll_create(nevents);
	if(ep == -1){
		vs_log_sys_error("epoll_create  fail:%s",strerror(errno) );
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

	timer = EPOLL_TIMEOUT;
	nums = epoll_wait( ep, event_list, nevents, timer );
	vs_time_update();
	vs_log_sys_info("epoll_wait ok %d",nums );
	if( nums < 0 ){
		switch( errno ){
			case EBADF:
			case EINVAL:
				vs_log_sys_error("epoll_wait error:%s", strerror(errno));
				return VS_ERROR;
		}
	}
	if( nums <= 0 ){
		return VS_OK;
	}

	for( i = 0; i < nums; ++i ){
		c = event_list[ i ].data.u64;
		if( c->fd == -1 ){
			continue;
		}
		events = event_list[ i ].events;
		if( events & (EPOLLERR|EPOLLHUP) ){
			vs_log_sys_error("epoll event error:%s", strerror(errno));
			vs_net_error_handle( c );
			continue;
		}

		rev = c->rev;
		if( ( events & EPOLLIN ) && rev->active ){
			if (rev->handle){
				rev->handle(rev);
			}
		}
        wev = c->wev;
		if( ( events & EPOLLOUT ) && wev->active ){
			if (wev->handle){
				wev->handle(wev);
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
int vs_event_add_conn( void* conn, int type)
{
	vs_conn_t*			  c;
	struct epoll_event	  ee;
	int					  etype;
    vs_event_t*		  rev;
	vs_event_t*		  wev;
	int do_type;

	c = conn;
	rev = c->rev;
	wev = c->wev;
	do_type = EPOLL_CTL_ADD;
	etype = EPOLLET;

	if (type == VS_EVENT_TYPE_READ) {
		if (rev->active == 1) {
			return VS_OK;
		}
		etype |= EPOLLIN;
		rev->active = 1;
		if (wev->active) {
			etype |= EPOLLOUT;
			do_type = EPOLL_CTL_MOD;
		}
	}
	else {			//VS_EVENT_TYPE_WRITE
		if (wev->active == 1) {
			return VS_OK;
		}
		etype |= EPOLLOUT;
		wev->active = 1;
		if (rev->active) {
			etype |= EPOLLIN;
			do_type = EPOLL_CTL_MOD;
		}
	}

	ee.events = etype;
	ee.data.u64 = c;
	if (epoll_ctl( ep, do_type, c->fd, &ee ) < 0){
		vs_log_sys_error("vs_event_add_conn epoll_ctl fail: %s",strerror(errno) );
		return VS_ERROR;
	}

	return VS_OK;
}
int vs_event_del_conn( void* conn, int type)
{
	vs_conn_t*					c;
	struct epoll_event			ee;
    vs_event_t					*rev, *wev;
	int							etype;
	int do_type;

	c = conn;
    rev = c->rev;
	wev = c->wev; 

	do_type = EPOLL_CTL_DEL;
	etype = EPOLLET;

	if (type == VS_EVENT_TYPE_READ) {
		if (rev->active == 0) {
			return VS_OK;
		}
		rev->active = 0;
		if (wev->active) {
			etype |= EPOLLOUT;
			do_type = EPOLL_CTL_MOD;
		}
	}
	else {			//VS_EVENT_TYPE_WRITE
		if (wev->active == 0) {
			return VS_OK;
		}
		wev->active = 0;
		if (rev->active) {
			etype |= EPOLLIN;
			do_type = EPOLL_CTL_MOD;
		}
	}
	ee.events = etype;
	ee.data.u64 = c;
	if (epoll_ctl( ep, do_type,  c->fd, &ee) < 0){
		vs_log_sys_error("vs_event_del_conn epoll_ctl fail: %s",strerror(errno) );
		return VS_ERROR;
	}
	return VS_OK;
}