#include "vs_conn_listen.h"
#include "vs_net.h"


int vs_tcp_listen_create(vs_cycle_t* cycle)
{
	int 					flag;
	int 					fd;
	struct sockaddr_in    	addr;
	vs_listen_t          	*listener;
	int 					port;
	char* 					ip;

	ip = TCP_IP;	
	port = TCP_PORT;


	fd = socket( AF_INET, SOCK_STREAM, 0 );
	if( fd<= 0 ){
		vs_log_sys_error("socket fail:%s", strerror(errno));
		return VS_ERROR;
	}
	vs_nonblocking(fd);
	vs_socket_reuse(fd);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr( ip );
	flag = bind( fd, ( struct sockaddr* ) &addr, sizeof( addr ) );
	if( flag ){
		close( fd );
		vs_log_sys_error("bind fail:%s",strerror(errno));
		return VS_ERROR;
	}
	flag = listen( fd, 128);
	if( flag ){
		close( fd );
		vs_log_sys_error("listen fail:%s", strerror(errno));
		return VS_ERROR;
	}


	listener = malloc(sizeof(vs_listen_t));
	listener->fd = fd;
	listener->ip = inet_addr( ip );
	listener->port = port;
	cycle->tcp_listener = listener;


	return VS_OK;
}
