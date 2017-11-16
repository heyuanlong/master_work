#include "vs_busi.h"
#include "vs_core.h"
#include "vs_busi_test.h"
#include "vs_interface.h"
#include "vs_net.h"

#include <string.h>

int ko_busi_handle( vs_busi_t* busi )
{
	void 			*data;
	vs_header_t 	*header;



	data = busi->c->recv_data + busi->c->recv_pre_size;
	header = data;

	switch(header->msg_cmd)
	{
		case TEST1:
			vs_busi_test_1(busi);
			break;
		case TEST2:
			vs_busi_test_2(busi);
			break;
		case TEST3:
			vs_busi_test_3(busi);
			break;
		case TEST4:
			vs_busi_test_4(busi);
			break;
		default:
			vs_log_error("invalid msg_cmd:%d",header->msg_cmd);

	}
	return VS_OK;
}


int ko_busi_conn_close_handle( int fd )
{
	vs_log_info("ko_busi_close_conn_handle :%d", fd);
	return VS_OK;
}
int ko_busi_conn_timeout_handle( vs_busi_t* busi )
{
	static int i = 0;
	
	char *buf = "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww";
	char buff[1024];
	int len;
	int j;

	vs_log_info("ko_busi_timeout_conn_handle :%d\n", busi->fd);
	//ko_busi_close_conn(busi);
	for ( j = 0; j < 10 ; j++)
	{
		++i;
		sprintf(buff, "%d_%s\n", i, buf);
		len = strlen(buff);
		vs_net_send_tcp(busi->c, buff, len);
	}
	return VS_OK;
}
//-----------------------------------------------

int ko_busi_close_conn(vs_busi_t* busi)
{
	vs_net_set_conn_close(busi->c);
}

vs_busi_t* 	vs_busi_get(int fd)
{
	vs_busi_t 			*t;
	t = malloc(sizeof(vs_busi_t));
	t->fd = fd;
	return t;
}

int vs_busi_free( vs_busi_t *busi )
{
	free(busi);
}


