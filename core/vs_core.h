
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


typedef struct 
{
	int			              fd;
	int                       port;
	unsigned long long        ip;
}vs_listen_t;

typedef struct{
	vs_listen_t* 	tcp_listener;
}vs_cycle_t;

typedef struct{
	int			size;
	void*   	data;
}vs_str_t;

#endif