#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vs_core.h"
#include "vs_process.h"
#include "vs_process_cycle.h"
#include "vs_config.h"
#include "vs_log.h"
#include "vs_signal.h"
#include "vs_daemon.h"
#include "vs_init_cycle.h"

int main(int argc, char const *argv[])
{
	vs_cycle_t   *cycle;

	if ((cycle = vs_cycle_create()) == NULL) {
		vs_log_sys_error("vs_cycle_create fail\n");
		return 0;
	}

	config_init();
	vs_time_init();

	vs_log_init();
	vs_log_set_level(VS_LOG_LEVEL_MAX);
	vs_log_set_path( "../log/");

#if 0
	vs_daemon();
#else 
	vs_log_set_path_for_printf();
#endif // 


	if (vs_init_cycle(cycle) != VS_OK){
		vs_log_sys_error("vs_init_cycle fail\n");
		return 0;
	}


	vs_master_process_cycle(cycle);

	return 0;
}


