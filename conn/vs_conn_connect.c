#include "vs_conn_connect.h"
#include "vs_socket.h"
#include "vs_event.h"
#include "vs_net.h"

vs_conn_t* vs_conn_tcp_connect(const char *ip,const int port)
{
	vs_conn_t*		c;
	int				fd;
	vs_event_t 		*rev, *wev;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		vs_log_sys_error("vs_conn_tcp_connect socket error:%d :%s", errno, strerror(errno));
		return NULL;
	}

	struct sockaddr_in sockAddr;
	bzero(&sockAddr, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(port);
	sockAddr.sin_addr.s_addr = inet_addr(ip);

	if (connect(fd, (struct sockaddr *)&sockAddr, sizeof(struct sockaddr)) == -1) {
		vs_log_sys_error("vs_conn_tcp_connect connect error:%d :%s", errno, strerror(errno));
		return NULL;
	}

	vs_nonblocking(fd);
	vs_socket_nodelay(fd);

	c = vs_conn_get(fd);
	if (NULL == c) {
		close(fd);
		vs_log_sys_error("vs_conn_tcp_connect vs_conn_get error");
		return NULL;
	}

	c->saddr = sockAddr;
	wev = c->wev;
	wev->handle = vs_net_send_handle;

	return c;
}