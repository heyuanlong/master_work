#include "vs_conn.h"
#include "vs_event.h"
#include "vs_event_timer.h"
#include "vs_log.h"
#include "vs_net.h"
#include "vs_busi.h"




int vs_conn_cycle_init(vs_cycle_t* cycle)
{
	
	if(vs_tcp_listen_create(&cycle->tcp_listener) != VS_OK){
		vs_log_sys_error("vs_tcp_listen_create fail");
		return VS_ERROR;
	}

	return VS_OK;
}

vs_conn_t* 			vs_conn_get( int fd )
{
	vs_conn_t* c;
	vs_event_t 				*rev, *wev;
	vs_event_timer_t 		*tev;
	vs_busi_t 				*busi;

	c = malloc(sizeof(vs_conn_t));
	c->fd = fd;
	c->busi =NULL;

	c->is_set_timeout = 0;
	c->timeout = 0;

	c->client_error = 0;
	c->client_shutdown = 0;
	c->server_shutdown = 0;

	rev = malloc(sizeof(vs_event_t));
	rev->fd = fd;
	rev->data = c;
	rev->active = 1;
	rev->handle = NULL;
	c->rev =rev;

	wev = malloc(sizeof(vs_event_t));
	wev->fd = fd;
	wev->data = c;
	wev->active = 1;
	wev->handle = NULL;
	c->wev =wev;

	tev = malloc(sizeof(vs_event_timer_t));
	tev->data = c;
	tev->handle = vs_net_timeout_handle;
	c->tev =tev;

	c->recv_data  = malloc(VS_CONN_RECV_BUF_SIZE);
	c->recv_pre_size = 0 ;
    c->recv_size = 0;
	c->recv_back_size = VS_CONN_RECV_BUF_SIZE;

	busi = vs_busi_get();
	busi->fd = fd;
	busi->c = c;
	c->busi =busi;
	
	
	
	

	printf("vs_conn_get :%d\n",fd);

	return c;
}
int vs_conn_free( vs_conn_t *c )
{
	free(c->rev);
	free(c->wev);
	free(c->tev);
	vs_busi_free(c->busi);
	free(c);
	return VS_OK;
}

