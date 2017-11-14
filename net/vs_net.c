
#include "vs_net.h"
#include "vs_conn_recv.h"
#include "vs_conn_send.h"
#include "vs_interface.h"
#include "vs_log.h"
#include "vs_busi.h"
#include "vs_event_timer.h"



int HEADER_SIZE = sizeof(vs_header_t);

int vs_net_recv_check( vs_conn_t  *c );
int vs_net_clean( vs_conn_t  *c );



int vs_net_module_init(vs_cycle_t* cycle)
{

	if(vs_conn_cycle_init(cycle) != VS_OK){
		printf("%s fail\n", "vs_conn_cycle_init");
		return VS_ERROR;
	}

	return VS_OK;
}


int vs_net_accept_handle( vs_event_t* ev )
{
	printf("%s\n","vs_net_accept_handle" );
	vs_conn_tcp_accept(ev);
}
int vs_net_read_handle( vs_event_t* ev )
{
	vs_conn_t*		c;
	int 			res;
	int 			res_check;
	/*    |-----header-------|------data----------|       */
	//适合ET模式处理

	c = ev->data;
	res = 0;

	vs_event_timer_add(c,TIMEOUTS);
	
	while(1){
		if(c->server_shutdown == 1){				//服务器关闭了conn
			vs_net_clean(c);
			break;
		}
		if(res == 0 || res == VS_CONN_RECV_BUF_FULL){
			res = vs_conn_recv(c);  				//尽可能的接受数据
		}

		res_check = vs_net_recv_check(c);
		if (res_check == VS_CONN_MSG_TOO_BIG){
			vs_log_sys_error("VS_CONN_MSG_TOO_BIG");
			c->server_shutdown = 1;
			continue;
		}else if (res_check == VS_CONN_MSG_NOT_ENOUGH){
			if (c->client_error == 1){
				vs_log_sys_error("client_error");
				c->server_shutdown = 1;
				continue;
			}
			if(c->client_shutdown == 1){
				vs_log_sys_error("client_shutdown");
				c->server_shutdown = 1;
				continue;
			}
			break;
		}else{

			ko_busi_handle(c->busi);
			c->recv_pre_size += res_check;
			c->recv_size -= res_check;
		}
	}
	return VS_OK;
}
int vs_net_send_handle( vs_event_t* ev )
{

	return VS_OK;
}

int vs_net_timeout_handle( vs_event_timer_t* ev )
{
	vs_conn_t*		c;

	c = ev->data;
	printf("vs_net_timeout_handle\n");

	if(c->server_shutdown == 1){				//服务器关闭了conn
		vs_net_clean(c);
		return VS_OK;
	}
	ko_busi_conn_timeout_handle(c->busi);
	return VS_OK;
}

//--------------------------------------------------------------
int vs_net_recv_check( vs_conn_t  *c )
{	
	int msg_size;
	if (c->recv_size >= HEADER_SIZE ){
		msg_size = *(int*)(c->recv_data + c->recv_pre_size);
		printf("msg_size :%d\n", msg_size);
		if(msg_size > VS_CONN_RECV_BUF_SIZE){
			return VS_CONN_MSG_TOO_BIG;
		}
		if (c->recv_size >= msg_size ){
			return msg_size;
		}
	}
	return VS_CONN_MSG_NOT_ENOUGH;
}

int vs_net_clean( vs_conn_t  *c )
{
	//清除 epoll
	//清除 event_timer
	//上层通知
	//释放内存

	int fd;
	fd = c->fd;
	printf("vs_net_clean :%d\n",fd);	
	
	vs_event_del_conn(c);
	vs_event_timer_del(c);
	vs_conn_free(c);
	ko_busi_conn_close_handle(fd);
	close(fd);
}
//-------------------------------------------------------------
int vs_net_set_conn_close(vs_conn_t *c)
{
	c->server_shutdown = 1;
	vs_event_timer_add(c,0);		//改为0秒超时。
}

