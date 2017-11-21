#include "vs_init_cycle.h"
#include "vs_net.h"

int vs_init_cycle(vs_cycle_t *cycle)
{
	vs_init_signals();

	vs_event_timer_init();

	if(vs_net_module_init(cycle) != VS_OK){
		vs_log_sys_error("vs_net_module_init fail");
		return VS_ERROR;
	}
	

}