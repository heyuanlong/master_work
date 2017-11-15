#ifndef __VS_EVENT_TIMER_H_
#define __VS_EVENT_TIMER_H_

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "vs_core.h"
#include "vs_conn.h"
#include "vs_event.h"
#include "vs_rbtree.h"

#define TIMEOUTS	2



typedef struct vs_event_timer_s vs_event_timer_t;
typedef int (*vs_event_timer_handle_ptr)( vs_event_timer_t *ev );
struct vs_event_timer_s
{
	vs_conn_t*					data;
	vs_event_timer_handle_ptr	handle;
};




int vs_event_timer_init();
int vs_event_timer_add( vs_conn_t* conn,  int timeout );
int vs_event_timer_del( vs_conn_t* conn );
void vs_event_timer_expire();


#endif