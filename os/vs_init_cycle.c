#include "vs_init_cycle.h"
#include "vs_net.h"
#include "vs_mem.h"


vs_cycle_t *	vs_cycle_create()
{
	vs_cycle_t *p;
	vs_pool_t  *pool;
	p = vs_alloc(sizeof(vs_cycle_t));
	if (p == NULL) {
		vs_log_sys_error("vs_alloc fail");
		return NULL;
	}

	pool = vs_create_pool(VS_MAX_ALLOC_FROM_POOL);
	if (pool == NULL) {
		vs_free(p);
		vs_log_sys_error("vs_create_pool fail");
		return NULL;
	}

	p->pool = pool;
	return p;
}

int vs_init_cycle(vs_cycle_t *cycle)
{
	vs_init_signals();

	vs_event_timer_init();

	if(vs_net_module_init(cycle) != VS_OK){
		vs_log_sys_error("vs_net_module_init fail");
		return VS_ERROR;
	}
	

}