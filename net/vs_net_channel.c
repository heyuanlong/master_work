#include "vs_net_channel.h"
#include "vs_conn.h"
#include "vs_process.h"
#include "string.h"



int vs_net_add_channel_event(int ch)
{
	vs_conn_t 				*c;
	vs_event_t 				*rev;

	c = vs_conn_get( ch );
	rev = c->rev;
	rev->handle = vs_net_read_channel_handle;
	rev->handle( rev );
	if (vs_event_add_conn( c, VS_EVENT_TYPE_READ) < 0){
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
		flag = recv( ev->fd, buf, size, 0 );
		if( flag < 0 ){
			if( errno == EAGAIN ){
				break;
			}
			else if( errno == EINTR ){
				continue;
			}
			vs_log_sys_error("recv channel VS_ERROR :%s",strerror(errno));
			return VS_ERROR;
		}
		if (flag == 0){
			//close
			return VS_OK;
		}
		if (flag < sizeof(vs_channel_t)){
			vs_log_sys_error("recv channel size fail");
			return VS_ERROR;
		}
		data = buf;
		vs_log_sys_info("vs_channel_t type:%d\n",data->type );
		if (data->type == 2){
			exit(0);
		}

	}
	return VS_OK;
}

//-----------------------------------------------------------------------
int vs_net_tell_child_quit()
{
	int i = 0;
	vs_channel_t data;
	data.type = 2;
	for (i; i < vs_last_process; ++i)
	{
		if (vs_processes[i].pid != -1) {
			vs_net_send_channel(vs_processes[i].channel[0], data);
			vs_log_sys_info("vs_net_send_channel pid:%d\n", vs_processes[i].pid);
		}
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
			vs_log_sys_error("vs_net_send_channel error:%d ,%s",errno,strerror(errno));
			return VS_ERROR;
		}
		send_len += flag;
	}
	return send_len;
}