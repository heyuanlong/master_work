
#ifndef _VS_TIME_H_INCLUDED_
#define _VS_TIME_H_INCLUDED_

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <vs_core.h>


void vs_time_init();
void vs_time_update();


extern ullong_t				vs_current_usec;			//微妙
extern ullong_t				vs_current_msec;			//毫秒
extern ullong_t				vs_current_sec;				//秒

extern volatile vs_str_t	vs_cached_err_log_utime;
extern volatile vs_str_t	vs_cached_err_log_mtime;
extern volatile vs_str_t	vs_cached_err_log_time;

extern volatile vs_str_t    vs_cached_time_format_yyyy_MM_dd;
extern volatile vs_str_t    vs_cached_time_format_yyyy_MM_dd_hh_mm_ss;

#endif