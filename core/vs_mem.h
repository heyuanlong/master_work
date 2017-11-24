#ifndef __VS_MEM_H_
#define __VS_MEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include "vs_core.h"

#define VS_MAX_ALLOC_FROM_POOL		10240
#define vs_inline      				inline
#define vs_free          			free




typedef void (*vs_pool_cleanup_pt)(void *data);
typedef struct vs_pool_cleanup_s  vs_pool_cleanup_t;
struct vs_pool_cleanup_s {
    vs_pool_cleanup_pt   handler;
    void                 *data;
    vs_pool_cleanup_t   *next;
};


typedef struct vs_pool_large_s  vs_pool_large_t;
struct vs_pool_large_s {
    vs_pool_large_t     *next;
    void                 *alloc;
};


typedef struct {
    u_char               *last;
    u_char               *end;
    vs_pool_t           *next;
    vs_uint_t            failed;
} vs_pool_data_t;


struct vs_pool_s {
    vs_pool_data_t       d;
	size_t 				block_nums;
	size_t 				large_nums;
    size_t                max;
    vs_pool_t           *current;
    vs_chain_t          *chain;
    vs_pool_large_t     *large;
    vs_pool_cleanup_t   *cleanup;
    vs_log_t            *log;
};


extern int 	block_nums;
extern int 	large_nums;

void *vs_alloc(size_t size);					//向系统分配内存
void *vs_calloc(size_t size);					//向系统分配内存


vs_pool_t *vs_create_pool(size_t size);			//创建pool
void vs_destroy_pool(vs_pool_t *pool);			//销毁pool
void vs_reset_pool(vs_pool_t *pool);				//重置pool，但是只释放了large内存

void *vs_palloc(vs_pool_t *pool, size_t size);				//申请
void *vs_palloc_just_large(vs_pool_t *pool, size_t size);		//只申请大块内存


vs_int_t vs_pfree(vs_pool_t *pool, void *p);					//只释放指定的大块内存





#endif