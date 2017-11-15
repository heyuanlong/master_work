#ifndef __VS_BUSI_H_
#define __VS_BUSI_H_

#include "vs_conn.h"

typedef struct {
	int 			fd;
	vs_conn_t		*c;
}vs_busi_t;


int ko_busi_handle( vs_busi_t* busi );
int ko_busi_conn_close_handle( int fd );
int ko_busi_conn_timeout_handle( vs_busi_t* busi );

//------------------------------------------------
int ko_busi_close_conn(vs_busi_t* busi);


vs_busi_t* 			vs_busi_get(int fd);
int 				vs_busi_free( vs_busi_t *busi );


#endif