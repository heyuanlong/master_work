
#include "vs_net.h"
#include "vs_conn_recv.h"
#include "vs_conn_send.h"
#include "vs_conn_accept.h"
#include "vs_conn_connect.h"
#include "vs_interface.h"
#include "vs_log.h"
#include "vs_event_timer.h"
#include "vs_event.h"


int HEADER_SIZE = sizeof(vs_header_t);

int vs_net_recv_check( vs_conn_t  *c );
int vs_net_clean( vs_conn_t  *c );
int vs_net_deal_send_chain(vs_conn_t  *c);


int vs_net_module_init(vs_cycle_t* cycle)
{

	if(vs_conn_cycle_init(cycle) != VS_OK){
		vs_log_sys_error("vs_conn_cycle_init fail");
		return VS_ERROR;
	}

	return VS_OK;
}

int vs_net_add_tcp_listen_event(int fd, void *listening)
{
	vs_conn_t    *c;
	vs_event_t	 *rev;

	c = vs_conn_get(fd);
	c->listening = listening;
	rev = c->rev;
	rev->handle = vs_net_listen_handle;
	vs_event_add_conn(c, VS_EVENT_TYPE_READ);

	return VS_OK;
}
int vs_net_add_udp_listen_event(int fd, void *listening)
{
	vs_conn_t    *c;
	vs_event_t	 *rev;

	c = vs_conn_get(fd);
	c->listening = listening;
	rev = c->rev;
	rev->handle = vs_net_listen_udp_handle;
	vs_event_add_conn(c, VS_EVENT_TYPE_READ);

	return VS_OK;
}
//--------------------------------------------
int vs_net_listen_handle( vs_event_t* ev )
{	
	vs_conn_tcp_accept(ev);
}
int vs_net_accept_handle(void *conn)
{
	vs_conn_t				 *c;
	vs_event_t 				*rev, *wev;

	c = conn;
	vs_event_timer_add(c, TIMEOUTS);

	rev = c->rev;
	wev = c->wev;
	rev->handle = vs_net_read_handle;
	wev->handle = vs_net_send_handle;
	//rev->handle( rev );
	if (vs_event_add_conn(c, VS_EVENT_TYPE_READ) < 0) {
		vs_log_sys_error("vs_event_add_conn fail");
		return VS_ERROR;
	}
	return VS_OK;
}
int vs_net_listen_udp_handle(vs_event_t* ev)
{
	vs_conn_tcp_accept_udp(ev);
}
int vs_net_accept_udp_handle(void *conn)
{
	vs_conn_t				 *c;

	c = conn;

	ko_busi_handle(c->busi);
	return VS_OK;
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
	vs_conn_t *c;
	c = ev->data;

	vs_net_deal_send_chain(c);
	if (c->send_chain == NULL) {		//不存在缓存链
		vs_event_del_conn(c, VS_EVENT_TYPE_WRITE);
	}
	return VS_OK;
}
int vs_net_error_handle(vs_conn_t *c)
{
	c->server_shutdown = 1;
	vs_net_clean(c);
}

int vs_net_timeout_handle( vs_event_timer_t* ev )
{
	vs_conn_t*		c;

	c = ev->data;
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
	
	vs_event_del_conn(c, VS_EVENT_TYPE_READ);
	vs_event_del_conn(c, VS_EVENT_TYPE_WRITE);
	vs_event_timer_del(c);
	vs_conn_free(c);
	ko_busi_conn_close_handle(fd);
	close(fd);
}
int vs_net_deal_send_chain(vs_conn_t  *c)
{
	//检查send_chain_nums，过大则报错
	vs_conn_send_chain_t	*chain;
	int						res;

	while (true)
	{
		if (c->send_chain == NULL) {
			break;
		}
		chain = c->send_chain;
		res = vs_conn_send(c->fd, chain->data, chain->size);
		if (res < 0) {
			return VS_CONN_SEND_ERROR;
		}
		chain->size -= res;
		if ( chain->size  > 0) {
			break;
		}
		else {
			c->send_chain = chain->next;
			c->send_chain_nums--;

			chain->next = c->send_spare_chain;
			c->send_spare_chain = chain;
			c->send_spare_nums++;
		}
	}
	return VS_OK;
}
//-------------------------------------------------------------
int vs_net_set_conn_close(vs_conn_t *c)
{
	c->server_shutdown = 1;
	vs_event_timer_add(c,0);		//改为0秒超时。
}

int vs_net_send_tcp(vs_conn_t *c, void * buf, int size)
{
	vs_conn_send_chain_t	*chain;
	int						res;
	int						remain;

	if (c->server_shutdown == 1 || c->client_error == 1 || c->client_shutdown == 1){
		return VS_CONN_HAVE_CLOSE;
	}

	if (c->send_chain != NULL) {		//存在缓存链
		if (vs_net_deal_send_chain(c) != VS_OK) {
			return VS_ERROR;
		}
	}
	if (c->send_chain != NULL) {		//存在缓存链
		if (c->send_chain_nums >= 10) {
			return VS_CONN_SEND_CHAIN_TOO_MUCH;
		}
		chain = vs_conn_send_chain_get(c, buf, size);
		vs_conn_send_chain_push(c, chain);
		vs_event_add_conn(c, VS_EVENT_TYPE_WRITE);		//add  wev
		return VS_OK;
	}
	//无缓存链
	//vs_log_sys_debug("vs_conn_send-------:%d",size);
	res = vs_conn_send(c->fd, buf, size);
	if (res < 0) {
		return VS_CONN_SEND_ERROR;
	}
	remain = size - res;
	if (remain > 0) {		//加入缓存链
		chain = vs_conn_send_chain_get(c, buf + res, remain);
		vs_conn_send_chain_push(c, chain);
		vs_event_add_conn(c, VS_EVENT_TYPE_WRITE);		//add  wev
	}
	return VS_OK;
}

vs_busi_t* vs_net_tcp_connect(const char *ip, const int port)
{
	vs_conn_t *c;
	c = vs_conn_tcp_connect(ip, port);
	if (c) {
		vs_net_send_tcp(c, "kkkkkkkk", strlen("kkkkkkkk"));
		vs_log_sys_debug("vs_net_send_tcp-------");
		return c->busi;
	}
	return NULL;
}

