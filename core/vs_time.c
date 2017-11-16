#include "vs_time.h"
 #include <time.h>

static unsigned char cached_err_log_utime[sizeof("1970/09/28 12:00:00.000000")];
static unsigned char cached_err_log_mtime[sizeof("1970/09/28 12:00:00.000")];
static unsigned char cached_err_log_time [sizeof("1970/09/28 12:00:00")];

static unsigned char cached_time_format_yyyy_MM_dd[sizeof("1970-09-28")];
static unsigned char cached_time_format_yyyy_MM_dd_hh_mm_ss[sizeof("1970-09-28 12:00:00")];


volatile vs_str_t	vs_cached_err_log_utime;
volatile vs_str_t	vs_cached_err_log_mtime;
volatile vs_str_t	vs_cached_err_log_time;

volatile vs_str_t    vs_cached_time_format_yyyy_MM_dd;
volatile vs_str_t    vs_cached_time_format_yyyy_MM_dd_hh_mm_ss;

ullong_t vs_current_usec; 			//微妙
ullong_t vs_current_msec;			//毫秒
ullong_t vs_current_sec;			//秒

void vs_time_init()
{
	vs_cached_err_log_utime.size = sizeof("1970/09/28 12:00:00.000000") - 1;
	vs_cached_err_log_mtime.size = sizeof("1970/09/28 12:00:00.000") - 1;
    vs_cached_err_log_time.size = sizeof("1970/09/28 12:00:00") - 1;
	vs_cached_time_format_yyyy_MM_dd.size = sizeof("1970-09-28") - 1;
    vs_cached_time_format_yyyy_MM_dd_hh_mm_ss.size = sizeof("1970-09-28 12:00:00") - 1;
	vs_time_update();
}

void vs_time_update()
{	
	time_t				sec;
	struct timeval   	tv;
	struct tm			tm, *ptm;
	unsigned char*  	p;

	gettimeofday( &tv, NULL );

	vs_current_sec = tv.tv_sec;
	vs_current_msec = tv.tv_usec / 1000;
	vs_current_usec = tv.tv_sec;

	sec = time(0);
	ptm = localtime( &sec );
	tm = *ptm;
	
	tm.tm_year += 1900;
	tm.tm_mon++;

	{
		p = cached_err_log_utime;
		sprintf(p, "%4d/%02d/%02d %02d:%02d:%02d.%06d",
            tm.tm_year, tm.tm_mon,
            tm.tm_mday, tm.tm_hour,
            tm.tm_min, tm.tm_sec,
            vs_current_usec);
        vs_cached_err_log_utime.data = p;
	}
	{
		p = cached_err_log_mtime;
		sprintf(p, "%4d/%02d/%02d %02d:%02d:%02d.%03d",
            tm.tm_year, tm.tm_mon,
            tm.tm_mday, tm.tm_hour,
            tm.tm_min, tm.tm_sec,
            vs_current_msec);
        vs_cached_err_log_mtime.data = p;
	}
	{
		p = cached_err_log_time;
		sprintf(p, "%4d/%02d/%02d %02d:%02d:%02d",
            tm.tm_year, tm.tm_mon,
            tm.tm_mday, tm.tm_hour,
            tm.tm_min, tm.tm_sec);
        vs_cached_err_log_time.data = p;
	}

	{
		p = cached_time_format_yyyy_MM_dd;
		sprintf(p, "%4d-%02d-%02d",
            tm.tm_year, tm.tm_mon,
            tm.tm_mday);
        vs_cached_time_format_yyyy_MM_dd.data = p;
	}
	{
		p = cached_time_format_yyyy_MM_dd_hh_mm_ss;
		sprintf(p, "%4d-%02d-%02d %02d:%02d:%02d",
            tm.tm_year, tm.tm_mon,
            tm.tm_mday, tm.tm_hour,
            tm.tm_min, tm.tm_sec);
        vs_cached_time_format_yyyy_MM_dd_hh_mm_ss.data = p;
	}

	return;
}