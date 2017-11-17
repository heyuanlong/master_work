#include "vs_conn_accept.h"
#include "vs_net.h"
#include "vs_event_timer.h"

int vs_conn_tcp_accept( vs_event_t* ev )
{
	int 					fd;
	struct sockaddr_in  	addr_in;
	int 					size;

	vs_conn_t 				*c;
	vs_event_t 				*rev, *wev;

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
		
		c = vs_conn_get( fd );
		if( NULL == c ){
			close( fd );
			return VS_ERROR;
		}
		vs_event_timer_add(c,TIMEOUTS);
		
		rev = c->rev;
		wev = c->wev;
		rev->handle = vs_net_read_handle;
		wev->handle = vs_net_send_handle;
		//rev->handle( rev );
		if (vs_event_add_conn( c , VS_EVENT_TYPE_READ) < 0){
			vs_log_sys_error("vs_event_add_conn fail");
			return VS_ERROR;
		}
		

	}while(1);
}