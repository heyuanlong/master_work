#include "vs_init_cycle.h"


int vs_init_cycle(vs_cycle_t *cycle)
{
	if(vs_net_module_init() != VS_OK){
		printf("%s fail\n", "vs_net_module_init");
		return VS_ERROR;
	}
	

}