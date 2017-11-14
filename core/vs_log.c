#include "vs_log.h"
#include "vs_core.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <unistd.h>         //access
#include <stdarg.h>         //vsnprintf,va_start,va_end
#include <sys/stat.h>       //mkdir
#include <sys/time.h>       //gettimeofday
#include <fcntl.h>




static int vs_log_sys_fd = 0;
static int vs_log_app_fd = 0;

static int vs_log_sys_size = 0;
static int vs_log_app_size = 0;

static int vs_log_level = VS_LOG_LEVEL_MAX;

static char  m_time_str[MAX_TIME_STR_LEN];
static char  path_str[ 256 ];
static int   path_len = 0;

char* get_time_str(int is_write);
int vs_log_real( int wfd, int pos, int level, const char* format, va_list arg_list );
int vs_log_turn_sys();
int vs_log_turn_app();


int vs_log_init()
{
	int						sys_fd;
	int						app_fd;
	
	char					buffer[256];
	int 					len = 0;
	const char* 			path = VS_LOG_PATH;
	char* 					pos;
	
	strcpy(buffer,path);
	len =  strlen(buffer);
	pos = buffer + len;
	
	strcpy(pos,"/ko_sys_log");
	sys_fd = open(buffer,O_WRONLY |O_CREAT|O_APPEND,0644);
	if( sys_fd <= 0 ){
		printf( "error log :%s could not be opened\n", buffer );
		return -1;
	}
	
	strcpy(pos,"/ko_app_log");
	app_fd = open(buffer,O_WRONLY |O_CREAT|O_APPEND,0644);
	if( app_fd <= 0 ){
		printf( "error log :%s could not be opened\n", buffer );
		return -1;
	}
	
	
	vs_log_sys_fd = sys_fd;
	vs_log_app_fd = app_fd;
	
	return 0;
}

int vs_log_set_level(int level)
{
	vs_log_level = level;
}

int vs_log_set_path( const char* path )
{
	strcpy(path_str,path);
	path_len = strlen(path_str);
	
	if( vs_log_turn_sys() ){
		return -1;
	}
	if( vs_log_turn_app() ){
		return -1;
	}
	return 0;
}

int vs_log_set_path_for_printf()
{
	vs_log_sys_fd = 0;
	vs_log_sys_size = 0;
	vs_log_app_fd = 0;
	vs_log_app_size = 0;
	
}

int vs_log_close()
{
	if( vs_log_sys_fd ){
		close( vs_log_sys_fd );
		vs_log_sys_fd = 0;
	}

	if( vs_log_app_fd ){
		close( vs_log_app_fd );
		vs_log_app_fd = 0;
	}

	return 0;
}

int vs_log_turn_sys()
{
	char*							buffer;
	char*							pos;
	int								sys_fd;
	
	buffer = path_str;
	sys_fd = 0;
	pos = buffer + path_len;
	
	sprintf(pos, "/ko_sys_log.%s", get_time_str(false));
	sys_fd = open(buffer,O_WRONLY|O_CREAT|O_TRUNC,0644);
	if( sys_fd <= 0 ){
		printf( "error log :%s could not be opened\n", buffer );
		return -1;
	}
	
	if (vs_log_sys_fd){
		close(vs_log_sys_fd);
	}

	vs_log_sys_fd = sys_fd;
	vs_log_sys_size = 0;
	
	return 0;
	
}
int vs_log_turn_app()
{
	char*							buffer;
	char*							pos;
	int								app_fd;
	
	buffer = path_str;
	app_fd = 0;
	pos = buffer + path_len;
	
	sprintf(pos, "/ko_app_log.%s", get_time_str(false));
	app_fd = open(buffer,O_WRONLY|O_CREAT|O_TRUNC,0644);
	if( app_fd <= 0 ){
		printf( "error log :%s could not be opened\n", buffer );
		return -1;
	}
	
	if (vs_log_app_fd){
		close(vs_log_app_fd);
	}

	vs_log_app_fd = app_fd;
	vs_log_app_size = 0;
	
	return 0;
}



int vs_log_real( int wfd, int pos, int level, const char* format, va_list arg_list )
{
	char 			buffer[VS_LOG_BUFFER_DEFAULT];
	char 			*p;
	int 			size;
	
	p = buffer;
	size = sprintf(p,"[%s][%s]", get_time_str(true), ko_level_str[level]);
	p += size;
	size += vsprintf(p , format, arg_list);
	if(size >= VS_LOG_BUFFER_DEFAULT){
		printf("log buf is too big!");
        buffer[VS_LOG_BUFFER_DEFAULT - 1] = 0;
        printf("log buf:%s", buffer);
        return -1;
	}
	buffer[size] = '\n';
	size += 1;
	if( wfd ){
		if ( pwrite( wfd, buffer, size , pos) == -1  ){
			printf("pwrite error\n");
		}
		return size;
	}else{
		buffer[size] = 0;
		printf(buffer);
		return 0;
	}
}


#define vs_log_start(format ,level,wfd,pos) \
	va_list arg_list; \
	if(level > vs_log_level ){	\
		return 0;	\
	} \
	va_start (arg_list,format); \
	size = vs_log_real(wfd,pos,level,format,arg_list);
	
#define vs_log_end va_end( arg_list );

#define vs_log_check_sys \
	vs_log_sys_size += size;	\
	if(vs_log_sys_size > VS_LOG_SIZE_LIMIT){ \
		vs_log_turn_sys(); \
	}
	
#define vs_log_check_app \
	vs_log_app_size += size; \
	if(vs_log_app_size > VS_LOG_SIZE_LIMIT){ \
		vs_log_turn_app(); \
	}
		
	
int vs_log_sys_real_debug(  const char* format, ... )
{
	int size;
	vs_log_start( format, VS_LOG_LEVEL_DEBUG, vs_log_sys_fd, vs_log_sys_size);
	vs_log_end;
	vs_log_check_sys;
}
int vs_log_sys_real_info(  const char* format, ... )
{
	int size;
	vs_log_start( format, VS_LOG_LEVEL_INFO, vs_log_sys_fd, vs_log_sys_size);
	vs_log_end;
	vs_log_check_sys;
}
int vs_log_sys_real_warn(  const char* format, ... )
{
	int size;
	vs_log_start( format, VS_LOG_LEVEL_WARN, vs_log_sys_fd, vs_log_sys_size);
	vs_log_end;
	vs_log_check_sys;
}
int vs_log_sys_real_error(  const char* format, ... )
{
	int size;
	vs_log_start( format, VS_LOG_LEVEL_ERROR, vs_log_sys_fd, vs_log_sys_size);
	vs_log_end;
	vs_log_check_sys;
}


int vs_log_app_real_debug(  const char* format, ... )
{
	int size;
	vs_log_start( format, VS_LOG_LEVEL_ERROR, vs_log_app_fd, vs_log_app_size);
	vs_log_end;
	vs_log_check_app;
}
int vs_log_app_real_info(  const char* format, ... )
{
	int size;
	vs_log_start( format, VS_LOG_LEVEL_INFO, vs_log_app_fd, vs_log_app_size);
	vs_log_end;
	vs_log_check_app;
}
int vs_log_app_real_warn(  const char* format, ... )
{
	int size;
	vs_log_start( format, VS_LOG_LEVEL_WARN, vs_log_app_fd, vs_log_app_size);
	vs_log_end;
	vs_log_check_app;
}
int vs_log_app_real_error(  const char* format, ... )
{
	int size;
	vs_log_start( format, VS_LOG_LEVEL_ERROR, vs_log_app_fd, vs_log_app_size);
	vs_log_end;
	vs_log_check_app;
}






char* get_time_str(int is_write)
{
    time_t now = {0};
    struct tm *ptime = NULL;
    time(&now);
    ptime = localtime(&now);
    memset(m_time_str, 0, sizeof(m_time_str));

    uint32 milisec = 0;
#ifdef WIN32
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    milisec = wtm.wMilliseconds;
#else
    struct timeval tv = {0};
    gettimeofday(&tv, 0);
    milisec = tv.tv_usec/1000;
#endif

    if (is_write)
    {//用来写日志
        sprintf(m_time_str, "%04d%02d%02d-%02d:%02d:%02d.%06ld",
            (1900+ptime->tm_year), (1+ptime->tm_mon), ptime->tm_mday,
            ptime->tm_hour, ptime->tm_min, ptime->tm_sec, milisec);
    }
    else
    {//用来重命名文件
        sprintf(m_time_str, "%04d-%02d-%02d-%02d_%02d_%02d_%06ld",
            (1900+ptime->tm_year), (1+ptime->tm_mon), ptime->tm_mday,
            ptime->tm_hour, ptime->tm_min, ptime->tm_sec, milisec);
    }

    return m_time_str;
}


















//