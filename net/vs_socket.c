
#include "vs_socket.h"



int vs_socket_reuse( int s )
{
    int reuseaddr = 1;
	setsockopt( s, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof( int ) );

	return VS_OK;
}



void vs_socket_nodelay( int s )
{
	int on;
	on = 1;
#ifndef __MACH__
	setsockopt( s, SOL_TCP, TCP_NODELAY, &on, sizeof (on));
#endif
}