
# master_work简介

master_work是一个由C开发的，方便快速搭建后端服务的系统。其进程模型（多进程单线程模型）是参照nginx构造的，一部分代码是从nginx里搬过来的，

## 如何使用：
 - make
 - cd build/master/bin/
 - ./masterbin (ctrl+c 重启子进程，ctrl+\ 关闭所有进程)
 - 客户端测试  ./clienttestnet 127.0.0.1 6001 2 5


## 理解master_work：

### 1. 进程模型
一个master进程，多个work进程，均为单线程。master通过unix socket跟work通信。

### 2. 轮训+超时
`os/vs_process_cycle.c`
```c
	for ( ; ; )
	{
		vs_time_update();
		vs_event_timer_expire();
		vs_process_events(cycle);
	}
```
`event/vs_event.c`
```c
int vs_process_events(vs_cycle_t *cycle)
{
	int                	nums;
	int					events;
	int 				timer;
	vs_conn_t 			*c;
	vs_event_t 			*rev;
	vs_event_t 			*wev;
	int 				i;

	timer = EPOLL_TIMEOUT;
	nums = epoll_wait( ep, event_list, nevents, timer );
	vs_time_update();
	vs_log_sys_info("epoll_wait ok %d",nums );
	...
	...
	...
 }
```

### 3. recv buf
每一个conn有一个接受数据的缓存区，看`conn/vs_conn.h`
<img src="./doc/img/recv.png">

### 4. send buf
每一个conn有2条发送相关的链表，待发送链和空闲链，看`conn/vs_conn.h`
<img src="./doc/img/send.png">

### 6. 内存池
`core/vs_mem.h core/vs_mem.c`是直接从nginx `ngx_palloc.h ngx_palloc.c` 简单改过来的。

## 待补充：
 - 读配置文件里读取配置参数
 - 预分配conn
