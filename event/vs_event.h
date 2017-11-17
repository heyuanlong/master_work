#ifndef __VS_EVENT_H_
#define __VS_EVENT_H_

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "vs_core.h"
#include "vs_conn.h"


#define VS_EVENT_TYPE_READ 			0
#define VS_EVENT_TYPE_WRITE 		1
#define EPOLL_TIMEOUT				500

typedef struct vs_event_s vs_event_t;
typedef int (*vs_event_handle_ptr)( vs_event_t *ev );
struct vs_event_s
{
	vs_conn_t*					data;
	int							fd;
	int							type;
	int							active;

	vs_event_handle_ptr			handle;
};


int vs_event_process_init(vs_cycle_t* cycle);
int vs_process_events(vs_cycle_t *cycle);

int vs_event_add( vs_event_t* ev, int type );
int vs_event_add_conn( void* conn,int type );

int vs_event_del( vs_event_t* ev, int type );
int vs_event_del_conn( void* conn, int type);


#endif