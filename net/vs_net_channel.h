#ifndef __VS_NET_CHANNEL_H_
#define __VS_NET_CHANNEL_H_

#include <unistd.h>
#include "vs_event.h"

typedef struct{
	int type;
	char data[32];
}vs_channel_t;


int vs_net_add_channel_event(int ch);
int vs_net_read_channel_handle( vs_event_t* ev );



int vs_net_tell_child_quit();
int vs_net_send_channel( int ch , vs_channel_t data );

#endif