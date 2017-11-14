#ifndef __VS_Log_h__
#define __VS_Log_h__


#ifndef VS_LOG_PATH
#define VS_LOG_PATH				"/tmp"
#endif

#define VS_LOG_BUFFER_DEFAULT			2048
#define VS_LOG_SIZE_LIMIT       		104857600			//100 * 1024 * 1024static int vs_log_error_fd = 0;
#define MAX_TIME_STR_LEN    			30
typedef unsigned long uint32;

enum {
    VS_LOG_LEVEL_ERROR     = 0,  //错误
    VS_LOG_LEVEL_WARN	   = 1,  //警告
    VS_LOG_LEVEL_INFO      = 2,  //普通
    VS_LOG_LEVEL_DEBUG     = 3,  //调试
    VS_LOG_LEVEL_MAX	   = 4,	 // 
};
static const char* ko_level_str[] = {"ERROR", "WARNING", "INFO", "DEBUG", "MAX"};



//系统日志
#define vs_log_sys_debug( format, args... )				vs_log_sys_real_debug( format, ##args )
#define vs_log_sys_info( format, args... )				vs_log_sys_real_info( format, ##args )
#define vs_log_sys_warn( format, args... )				vs_log_sys_real_warn( format, ##args )
#define vs_log_sys_error( format, args... )				vs_log_sys_real_error( format, ##args )

//应用日志
#define vs_log_debug( format, args... )					vs_log_app_real_debug( format, ##args )
#define vs_log_info( format, args... )					vs_log_app_real_info( format, ##args )
#define vs_log_warn( format, args... )					vs_log_app_real_warn( format, ##args )
#define vs_log_error( format, args... )					vs_log_app_real_error( format, ##args )




int vs_log_sys_real_debug( const char* format, ... );
int vs_log_sys_real_info( const char*  format, ... );
int vs_log_sys_real_warn(  const char* format, ... );
int vs_log_sys_real_error( const char*  format, ... );


int vs_log_app_real_debug( const char*  format, ... );
int vs_log_app_real_info(  const char* format, ... );
int vs_log_app_real_warn(  const char* format, ... );
int vs_log_app_real_error(  const char* format, ... );


int vs_log_init();
int vs_log_set_level(int level);
int vs_log_set_path( const char* path );
int vs_log_set_path_for_printf();
int vs_log_close();




#endif // __VS_Log_h__