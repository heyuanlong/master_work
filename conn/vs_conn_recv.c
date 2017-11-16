#include "vs_conn_recv.h"
#include "vs_log.h"
#include "vs_interface.h"


int vs_conn_recv( vs_conn_t  *c)
{
	int flag;
	int tmp_size;
	while( 1 ){
		flag = recv( c->fd, c->recv_data + c->recv_pre_size + c->recv_size, c->recv_back_size, 0 );
		if( flag < 0 ){
			if( errno == EAGAIN ){
				return VS_CONN_RECV_AGAIN;
			}
			else if( errno == EINTR ){
				continue;
			}
			vs_log_sys_error("recv client error:%d :%s", errno,strerror(errno));
			c->client_error = 1;
			return VS_CONN_RECV_ERROR;
		}
		if (flag == 0){
			vs_log_sys_error("recv client close");
			c->client_shutdown = 1;
			return VS_CONN_CLIENT_CLOSE;
		}
		tmp_size =c->recv_back_size;

		c->recv_size += flag;
		c->recv_back_size -= flag;
		if (c->recv_back_size == 0 && c->recv_pre_size > 0){
			memcpy(c->recv_data,c->recv_data + c->recv_pre_size ,c->recv_size);	//移动数据
			c->recv_back_size += c->recv_pre_size;
			c->recv_pre_size = 0;
		}else{
			if( flag <= tmp_size ){
	            return VS_CONN_RECV_BUF_FULL;	//可能是数据包太少，net层会检测
	        }			
		}
	}
	//return VS_CONN_RECV_OK;			//不会走到这一步
}

