
#ifndef _VS_CORE_H_INCLUDED_
#define _VS_CORE_H_INCLUDED_

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "vs_log.h"
#include "vs_config.h"


#define  true          	1
#define  false          0

#define  VS_OK          0
#define  VS_ERROR      -1

typedef  unsigned long long			ullong_t;

typedef int(*vs_listen_handle_ptr)(void  *conn);
typedef struct vs_listen_s vs_listen_t; 
struct vs_listen_s{
	int			              fd;
	int                       port;
	unsigned long long        ip;
	int						  type;
	vs_listen_handle_ptr	  handle;
	vs_listen_t				  *next;

};

typedef struct{
	vs_listen_t* 	listener;
}vs_cycle_t;

typedef struct{
	int			size;
	void*   	data;
}vs_str_t;

#endif