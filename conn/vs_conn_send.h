#ifndef __VS_CONN_SEND_H_
#define __VS_CONN_SEND_H_

#include <unistd.h>

#include "vs_conn.h"





int vs_conn_send(int fd,void *buf,int size);
int vs_conn_send_udp_simple(int fd, void *buf, int size, const char *ip, const int port);


#endif