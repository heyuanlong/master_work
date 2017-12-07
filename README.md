
# master_work���

master_work��һ����C�����ģ�������ٴ��˷����ϵͳ�������ģ�ͣ�����̵��߳�ģ�ͣ��ǲ���nginx����ģ�һ���ִ����Ǵ�nginx�������ģ�

## ���ʹ�ã�
 - make
 - cd build/master/bin/
 - ./masterbin (ctrl+c �����ӽ��̣�ctrl+\ �ر����н���)
 - �ͻ��˲���  ./clienttestnet 127.0.0.1 6001 2 5


## ���master_work��

### 1. ����ģ��
һ��master���̣����work���̣���Ϊ���̡߳�masterͨ��unix socket��workͨ�š�

### 2. ��ѵ+��ʱ
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
ÿһ��conn��һ���������ݵĻ���������`conn/vs_conn.h`
<img src="./doc/img/recv.png">

### 4. send buf
ÿһ��conn��2��������ص��������������Ϳ���������`conn/vs_conn.h`
<img src="./doc/img/send.png">

### 6. �ڴ��
`core/vs_mem.h core/vs_mem.c`��ֱ�Ӵ�nginx `ngx_palloc.h ngx_palloc.c` �򵥸Ĺ����ġ�

## �����䣺
 - �������ļ����ȡ���ò���
 - Ԥ����conn
