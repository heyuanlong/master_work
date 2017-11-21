#ifndef __VS_CONN_LISTEN_ACCEPT_H_
#define __VS_CONN_LISTEN_ACCEPT_H_

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "vs_conn.h"
#include "vs_socket.h"
#include "vs_event.h"

int vs_tcp_listen_create(vs_cycle_t* cycle ,const char *ip,const int port, vs_listen_handle_ptr handle);
int vs_udp_listen_create(vs_cycle_t* cycle, const char *ip, const int port, vs_listen_handle_ptr handle);

#endif