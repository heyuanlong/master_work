#ifndef __VS_CONN_H_
#define __VS_CONN_H_

#include <unistd.h>
#include "vs_core.h"
#include "vs_rbtree.h"


#define VS_CONN_RECV_OK				     0
#define VS_CONN_RECV_ERROR				 -1
#define VS_CONN_RECV_AGAIN				 -2
#define VS_CONN_CLIENT_CLOSE			 -3
#define VS_CONN_RECV_BUF_FULL			 -4
#define VS_CONN_MSG_TOO_BIG			 	 -5
#define VS_CONN_MSG_NOT_ENOUGH			 -6


#define VS_CONN_RECV_BUF_SIZE			1024

typedef struct vs_conn_s vs_conn_t;
typedef struct vs_conn_s
{
	int			              fd;

	void 					*recv_data;
	int 					recv_pre_size;
    int 					recv_size;
	int 					recv_back_size;

	int 					client_error;		//客户端出错
	int 					client_shutdown;	//客户端close
	int 					server_shutdown;	//服务端close

	void 					*busi; 				//vs_busi_t*

	int 					is_set_timeout;
	int 					timeout; 			//连接超时时间
	vs_rbtree_node_t		timer_node;


	void                    *send_buf;
	void                    *rev; 				//vs_event_t*
	void                    *wev; 				//vs_event_t*
	void                    *tev; 				//vs_event_timer_t*

};

int 				vs_conn_cycle_init(vs_cycle_t* cycle);
vs_conn_t* 			vs_conn_get( int fd );
int 				vs_conn_free( vs_conn_t *c );



#endif