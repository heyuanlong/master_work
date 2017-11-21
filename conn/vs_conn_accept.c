#include "vs_conn_accept.h"
#include "vs_net.h"
#include "vs_event_timer.h"

int vs_conn_tcp_accept( vs_event_t* ev )
{
	int 					fd;
	struct sockaddr_in  	addr_in;
	int 					size;

	vs_conn_t 				*c,*new_c;
	vs_listen_t				*ls;

	c = ev->data;
	ls = c->listening;
	do{
		fd = accept( ev->fd, ( struct sockaddr* )&addr_in, ( socklen_t* ) &size );
		if( fd < 0 ){
			switch( errno ){
				case EAGAIN:
					return VS_OK;
				case EINTR:
				case ECONNABORTED:
					continue;
			}
			vs_log_sys_error("accept error:%d :%s", errno, strerror(errno));
			return VS_ERROR;
		}
		vs_nonblocking(fd);
		vs_socket_nodelay( fd );
		
		new_c = vs_conn_get( fd );
		if( NULL == new_c){
			close( fd );
			vs_log_sys_error("accept vs_conn_get error");
			return VS_ERROR;
		}
		new_c->saddr = addr_in;

		if (ls->handle(new_c) < 0){
			vs_log_sys_error("vs_net_accept_handle fail");
			return VS_ERROR;
		}
		
	}while(1);
}

int vs_conn_tcp_accept_udp(vs_event_t* ev)
{
	struct sockaddr_in  	addr_in;
	int 					flag;
	int						len;

	vs_conn_t 				*c;
	vs_listen_t				*ls;

	c = ev->data;
	ls = c->listening;

	while (1) {
		len = sizeof(struct sockaddr_in);
		flag = recvfrom(c->fd, c->recv_data, c->recv_back_size, 0, (struct sockaddr*)&addr_in, &len);
		if (flag <= 0) {
			break;
		}

		c->recv_addr = addr_in;
		ls->handle(c);
	}
}