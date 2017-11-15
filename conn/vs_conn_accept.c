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
			printf("%s fail\n","accept" );
			return VS_ERROR;
		}
		vs_nonblocking(fd);
		vs_socket_nodelay( c->fd );
		
		c = vs_conn_get( fd );
		if( NULL == c ){
			close( fd );
			printf("%s fail\n","vs_conn_get" );
			//ko_log_error( "cannot get conn in accept" );
			return VS_ERROR;
		}
		vs_event_timer_add(c,TIMEOUTS);
		
		rev = c->rev;
		wev = c->wev;
		rev->handle = vs_net_read_handle;
		wev->handle = vs_net_send_handle;
		//rev->handle( rev );
		if (vs_event_add_conn( c , VS_EVENT_TYPE_READ) < 0){
			printf("%s fail\n", "vs_event_add_conn");
			return VS_ERROR;
		}
		

	}while(1);
}