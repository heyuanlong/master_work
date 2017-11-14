#ifndef __VS_SOCKET_H_
#define __VS_SOCKET_H_

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


#include "vs_core.h"

#define vs_nonblocking(s)  fcntl(s, F_SETFL, fcntl(s, F_GETFL) | O_NONBLOCK)
#define vs_blocking(s)     fcntl(s, F_SETFL, fcntl(s, F_GETFL) & ~O_NONBLOCK)


int vs_socket_reuse( int s );
void vs_socket_nodelay( int s );

#endif