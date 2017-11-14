#include "vs_net_channel.h"
#include "vs_conn.h"
#include "vs_process.h"
#include "string.h"



int vs_net_add_channel_event(int ch)
{
	vs_conn_t 				*c;
	vs_event_t 				*rev, *wev;

	c = vs_conn_get( ch );
	if( NULL == c ){
		close( ch );
		printf("%s fail\n","vs_conn_get" );
		//ko_log_error( "cannot get conn in accept" );
		return VS_ERROR;
	}

	rev = c->rev;
	wev = c->wev;
	rev->handle = vs_net_read_channel_handle;
	wev->handle = NULL;
	rev->handle( rev );
	if (vs_event_add_conn( c ) < 0){
		printf("%s fail\n", "vs_event_add_conn");
		return VS_ERROR;
	}

	return VS_OK;
}

int vs_net_read_channel_handle( vs_event_t* ev )
{
	int flag;
	char buf[512];
	int size;
	vs_channel_t *data;

	size = sizeof(buf);

	while( 1 ){
		memset(buf,0,size);
		printf("recv channel fd:%d \n",ev->fd );
		flag = recv( ev->fd, buf, size, 0 );
		if( flag < 0 ){
			if( errno == EAGAIN ){
				break;
			}
			else if( errno == EINTR ){
				continue;
			}
			printf("%s VS_ERROR\n","recv channel");
			return VS_ERROR;
		}
		if (flag == 0){
			//close
			return VS_OK;
		}
		if (flag < sizeof(vs_channel_t)){
			printf("%s VS_ERROR\n","recv channel size");
			return VS_ERROR;
		}
		data = buf;
		printf("vs_channel_t:%d\n",data->type );
		if (data->type == 2){
			exit(0);
		}

	}
	return VS_OK;
}

int vs_net_tell_child_quit()
{
	int i = 0;
	vs_channel_t data;
	data.type = 2;
	for (i; i < vs_last_process; ++i)
	{
	    vs_net_send_channel(vs_processes[i].channel[0],data);
	}
	return VS_OK;
}


int vs_net_send_channel( int ch , vs_channel_t data )
{
	const void* buf = &data;
	int size = sizeof(vs_channel_t);

	int flag = 0;
	int send_len = 0;
	while(size > send_len){
		flag = send(ch,buf + send_len,size - send_len,0);
		if( flag < 0 ){
			switch( errno ){
				case EINTR:
					continue;
				case EAGAIN:
					return send_len;
			}
			printf("vs_net_send_channel error:%d ,%s",errno,strerror(errno));
			return VS_ERROR;
		}
		send_len += flag;
	}
	return send_len;
}