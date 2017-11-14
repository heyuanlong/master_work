#ifndef __VS_CONN_LISTEN_ACCEPT_H_
#define __VS_CONN_LISTEN_ACCEPT_H_

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "vs_conn.h"
#include "vs_socket.h"
#include "vs_event.h"

int vs_conn_tcp_accept( vs_event_t* ev );

#endif