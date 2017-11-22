#ifndef __VS_CONN_H_
#define __VS_CONN_H_

#include <unistd.h>
#include "vs_core.h"
#include "vs_rbtree.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define VS_CONN_RECV_OK				     0
#define VS_CONN_RECV_ERROR				 -1
#define VS_CONN_RECV_AGAIN				 -2
#define VS_CONN_CLIENT_CLOSE			 -3
#define VS_CONN_RECV_BUF_FULL			 -4

#define VS_CONN_MSG_TOO_BIG			 	 -5
#define VS_CONN_MSG_NOT_ENOUGH			 -6

#define VS_CONN_SEND_ERROR				 -7
#define VS_CONN_SEND_AGAIN				 -8
#define VS_CONN_HAVE_CLOSE				 -9

#define VS_CONN_SEND_CHAIN_TOO_MUCH		-10				//发送缓存链过多

#define VS_CONN_RECV_BUF_SIZE			1024
#define VS_CONN_SEND_BUD_SIZE			1024

#define TCP_IP							"0.0.0.0"
#define TCP_PORT						6000
#define UDP_PORT						6001

typedef struct vs_conn_send_chain_s vs_conn_send_chain_t;
struct vs_conn_send_chain_s
{
	void						*data;
	int							size;
	int							allsize;
	vs_conn_send_chain_t		*next;
};

typedef struct vs_conn_s vs_conn_t;
struct vs_conn_s
{
	int			            fd;
	struct sockaddr_in		saddr;
	struct sockaddr_in		recv_addr;

	void 					*recv_data;
	int 					recv_pre_size;
    int 					recv_size;
	int 					recv_back_size;

	vs_conn_send_chain_t	*send_chain;
	int						send_chain_nums;
	vs_conn_send_chain_t	*send_spare_chain;
	int						send_spare_nums;

	int 					client_error;		//客户端出错
	int 					client_shutdown;	//客户端close
	int 					server_shutdown;	//服务端close

	void 					*busi; 				//vs_busi_t*

	int 					is_set_timeout;
	int 					timeout; 			//连接超时时间
	vs_rbtree_node_t		timer_node;
	void                    *tev; 				//vs_event_timer_t*

	void                    *rev; 				//vs_event_t*
	void                    *wev; 				//vs_event_t*

	void					*listening;
};

int 				vs_conn_cycle_init(vs_cycle_t* cycle);
vs_conn_t* 			vs_conn_get( int fd );
int 				vs_conn_free( vs_conn_t *c );

vs_conn_send_chain_t*  vs_conn_send_chain_get(vs_conn_t *c, void * buf,int size);
int					   vs_conn_send_chain_push(vs_conn_t *c, vs_conn_send_chain_t* chain);
void 				   vs_conn_send_chain_free(vs_conn_t *c);


#endif