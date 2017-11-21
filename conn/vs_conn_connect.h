#ifndef __VS_CONN_CONNECT_H_
#define __VS_CONN_CONNECT_H_

#include <unistd.h>

#include "vs_conn.h"

vs_conn_t* vs_conn_tcp_connect(const char *ip,const int port );



#endif