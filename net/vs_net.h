#ifndef __VS_NET_H_
#define __VS_NET_H_

#include <unistd.h>

#include "vs_event.h"
#include "vs_event_timer.h"
#include "vs_core.h"
#include "vs_busi.h"

//----------------------初始化----------------------------------------
int					vs_net_module_init(vs_cycle_t* cycle);
int					vs_net_add_tcp_listen_event(int fd ,void *listening);
int					vs_net_add_udp_listen_event(int fd, void *listening);

//------------------------监听，接收回调--------------------------------------
int					vs_net_listen_handle( vs_event_t* ev );		
int					vs_net_listen_udp_handle(vs_event_t* ev);
int					vs_net_accept_handle(void *conn);
int					vs_net_accept_udp_handle(void *conn);

//------------------------读写，错误，超时回调--------------------------------------
int					vs_net_read_handle( vs_event_t* ev );
int					vs_net_send_handle( vs_event_t* ev );
int					vs_net_timeout_handle( vs_event_timer_t* ev );
int					vs_net_error_handle(vs_conn_t *c);

//--------------------------主动调用------------------------------------
int					vs_net_set_conn_close(vs_conn_t *c);
int					vs_net_send_tcp(vs_conn_t *c,void * buf,int size);
int					vs_net_send_udp_simple(int fd, void *buf, int size, const char *ip, const int port);
vs_busi_t*			vs_net_tcp_connect(const char *ip, const int port);


#endif