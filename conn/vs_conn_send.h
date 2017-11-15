#ifndef __VS_CONN_SEND_H_
#define __VS_CONN_SEND_H_

#include <unistd.h>

#include "vs_conn.h"





int vs_conn_send(int fd,void *buf,int size);



#endif