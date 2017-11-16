#include "vs_init_cycle.h"


int vs_init_cycle(vs_cycle_t *cycle)
{
	if(vs_net_module_init() != VS_OK){
		vs_log_sys_error("vs_net_module_init fail");
		return VS_ERROR;
	}
	

}