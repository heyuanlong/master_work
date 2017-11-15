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

	c->client_error = 0;
	c->client_shutdown = 0;
	c->server_shutdown = 0;

	rev = malloc(sizeof(vs_event_t));
	rev->fd = fd;
	rev->data = c;
	rev->active = 0;
	rev->handle = NULL;
	rev->type = VS_EVENT_TYPE_READ;
	c->rev =rev;

	wev = malloc(sizeof(vs_event_t));
	wev->fd = fd;
	wev->data = c;
	wev->active = 0;
	wev->handle = NULL;
	wev->type = VS_EVENT_TYPE_WRITE;
	c->wev =wev;


	tev = malloc(sizeof(vs_event_timer_t));
	tev->data = c;
	tev->handle = vs_net_timeout_handle;
	c->tev =tev;
	c->is_set_timeout = 0;
	c->timeout = 0;

	c->recv_data  = malloc(VS_CONN_RECV_BUF_SIZE);
	c->recv_pre_size = 0 ;
    c->recv_size = 0;
	c->recv_back_size = VS_CONN_RECV_BUF_SIZE;

	c->send_chain = NULL;
	c->send_chain_nums = 0;
	c->send_spare_chain = NULL;
	c->send_spare_nums = 0;

	busi = vs_busi_get(fd);
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
	free(c->recv_data);

	vs_busi_free(c->busi);
	vs_conn_send_chain_free(c);
	free(c);
	return VS_OK;
}



vs_conn_send_chain_t*  vs_conn_send_chain_get(vs_conn_t *c, void * buf,int size)
{
	vs_conn_send_chain_t* chain = NULL;
	vs_conn_send_chain_t  **pch;

	pch = &c->send_spare_chain;
	while (*pch != NULL) {
		if ( (*pch)->allsize >= size) {
			break;
		}
		pch = &(*pch)->next;
	}
	if (*pch == NULL) {
		chain = malloc(sizeof(vs_conn_send_chain_t));
		chain->data = malloc(size);
		chain->size = size;
		chain->allsize = size;
		chain->next = NULL;
		memcpy(chain->data, buf, size);
		printf("--------------------------------vs_conn_send_chain_get malloc \n");
	}
	else {
		chain = *pch;
		*pch = chain->next;

		chain->size = size;
		chain->next = NULL;
		memcpy(chain->data, buf, size);
		c->send_spare_nums--;
		printf("--------------------------------vs_conn_send_chain_get spare \n");
	}

	return chain;
}
int	vs_conn_send_chain_push(vs_conn_t *c, vs_conn_send_chain_t* chain)
{
	vs_conn_send_chain_t  **pch;

	printf("--------------------------------vs_conn_send_chain_push \n");
	chain->next = NULL;
	pch = &c->send_chain;
	while (*pch != NULL) {
		pch = &(*pch)->next;
	}
	*pch = chain;
	c->send_chain_nums++;

	return VS_OK;
}
void   vs_conn_send_chain_free(vs_conn_t *c)
{
	vs_conn_send_chain_t  *ch, *nch;
	nch = ch = c->send_chain;
	while (ch != NULL) {
		nch = ch->next;
		free(ch);
		ch = nch;
	}
	c->send_chain_nums = 0;
	nch = ch = c->send_spare_chain;
	while (ch != NULL) {
		nch = ch->next;
		free(ch);
		ch = nch;
	}
	return;
}