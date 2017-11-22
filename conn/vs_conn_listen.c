#include "vs_conn_listen.h"
#include "vs_net.h"

static void vs_conn_cycle_add_listen(vs_cycle_t* cycle, vs_listen_t *listener);

int vs_tcp_listen_create(vs_cycle_t* cycle, const char *ip, const int port, vs_listen_handle_ptr handle)
{
	int 					flag;
	int 					fd;
	struct sockaddr_in    	addr;
	vs_listen_t          	*listener;

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
	listener->type = SOCK_STREAM;
	listener->ip = inet_addr( ip );
	listener->port = port;
	listener->handle = handle;

	vs_conn_cycle_add_listen(cycle, listener);
	return VS_OK;
}

int vs_udp_listen_create(vs_cycle_t* cycle, const char *ip, const int port, vs_listen_handle_ptr handle)
{
	int 					flag;
	int 					fd;
	struct sockaddr_in    	addr;
	vs_listen_t          	*listener;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd <= 0) {
		vs_log_sys_error("vs_udp_listen_create socket fail:%s", strerror(errno));
		return VS_ERROR;
	}
	vs_nonblocking(fd);
	vs_socket_reuse(fd);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	flag = bind(fd, (struct sockaddr*) &addr, sizeof(addr));
	if (flag) {
		close(fd);
		vs_log_sys_error("vs_udp_listen_create bind fail:%s", strerror(errno));
		return VS_ERROR;
	}


	listener = malloc(sizeof(vs_listen_t));
	listener->fd = fd;
	listener->type = SOCK_DGRAM;
	listener->ip = inet_addr(ip);
	listener->port = port;
	listener->handle = handle;

	vs_conn_cycle_add_listen(cycle, listener);

	return VS_OK;
}
static void vs_conn_cycle_add_listen(vs_cycle_t* cycle, vs_listen_t *listener)
{
	vs_listen_t  **plisten;

	listener->next = NULL;
	plisten = &cycle->listener;
	while (*plisten != NULL) {
		plisten = &(*plisten)->next;
	}
	*plisten = listener;

	return;
}