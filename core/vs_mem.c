#include "vs_mem.h"
#include "vs_log.h"


static vs_inline void *vs_palloc_small(vs_pool_t *pool, size_t size);	//申请小块内存
static void *vs_palloc_block(vs_pool_t *pool, size_t size);				//看实现里的注释
static void *vs_palloc_large(vs_pool_t *pool, size_t size);				//申请大块内存




void *vs_alloc(size_t size)
{
    void  *p;
    p = malloc(size);
    if (p == NULL) {
		vs_log_sys_error("malloc fail");
    }
    return p;
}


void *vs_calloc(size_t size)
{
    void  *p;
    p = vs_alloc(size);
    if (p) {
        memset(p,0, size);
    }
    return p;
}


vs_pool_t *vs_create_pool(size_t size)
{
    vs_pool_t  *p;

    p = vs_alloc(size);
    if (p == NULL) {
		vs_log_sys_error("vs_alloc fail");
        return NULL;
    }

    p->d.last = (u_char *) p + sizeof(vs_pool_t);
    p->d.end = (u_char *) p + size;
    p->d.next = NULL;
    p->d.failed = 0;

    size = size - sizeof(vs_pool_t);
    p->max = (size < VS_MAX_ALLOC_FROM_POOL) ? size : VS_MAX_ALLOC_FROM_POOL;

    p->current = p;
	p->block_nums=1;
	p->large_nums=0;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = NULL;

    return p;
}


void vs_destroy_pool(vs_pool_t *pool)
{
    vs_pool_t          *p, *n;
    vs_pool_large_t    *l;
    vs_pool_cleanup_t  *c;

    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler) {
            c->handler(c->data);
        }
    }

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            vs_free(l->alloc);
        }
    }

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        vs_free(p);
        if (n == NULL) {
            break;
        }
    }
}


void vs_reset_pool(vs_pool_t *pool)
{
    vs_pool_t        *p;
    vs_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            vs_free(l->alloc);
			pool->large_nums--;
        }
    }

    for (p = pool; p; p = p->d.next) {
        //p->d.last = (u_char *) p + sizeof(vs_pool_t);		//nginx 就是这样，表示有疑问
		p->d.last = (u_char *) p +  (p == pool ? sizeof(vs_pool_t ) : sizeof(vs_pool_data_t));
        p->d.failed = 0;
    }

    pool->current = pool;
    pool->chain = NULL;
    pool->large = NULL;
}


void *
vs_palloc(vs_pool_t *pool, size_t size)
{
	//可以在这里扩展，分配一个结构体返回，用于区分是从small还是large里返回。
	if(size <= 0 ){
		return NULL;
	}
    if (size <= pool->max) {
        return vs_palloc_small(pool, size);
    }

    return vs_palloc_large(pool, size);
}



static vs_inline void *
vs_palloc_small(vs_pool_t *pool, size_t size)
{
    u_char      *m;
    vs_pool_t  *p;

    p = pool->current;

    do {
        m = p->d.last;

        if ((size_t) (p->d.end - m) >= size) {
            p->d.last = m + size;
            return m;
        }
        p = p->d.next;
    } while (p);

    return vs_palloc_block(pool, size);
}


static void *
vs_palloc_block(vs_pool_t *pool, size_t size)
{
	//(这里有个需要注意的地方，当当前内存节点的剩余空间不够分配时，nginx会重新创建一个vs_pool_t对象，并且将pool.d->next指向新的vs_pool_t,新分配的vs_pool_t对象只用到了vs_pool_data_t区域，并没有头部信息，头部信息部分已经被当做内存分配区域了)
    u_char      *m;
    size_t       psize;
    vs_pool_t  *p, *new;

    psize = (size_t) (pool->d.end - (u_char *) pool);

    m = vs_alloc(psize);
    if (m == NULL) {
		vs_log_sys_error("vs_alloc fail");
        return NULL;
    }

    new = (vs_pool_t *) m;

    new->d.end = m + psize;
    new->d.next = NULL;
    new->d.failed = 0;

    m += sizeof(vs_pool_data_t);
    new->d.last = m + size;

    for (p = pool->current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > 4) {
            pool->current = p->d.next;
        }
    }

    p->d.next = new;					//尾插法
	pool->block_nums++;
    return m;
}


static void *
vs_palloc_large(vs_pool_t *pool, size_t size)
{
    void              *p;
    vs_uint_t         n;
    vs_pool_large_t  *large;

    p = vs_alloc(size);
    if (p == NULL) {
		vs_log_sys_error("vs_alloc fail");
        return NULL;
    }

    n = 0;

    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }

        if (n++ > 3) {
            break;
        }
    }

    large = vs_palloc_small(pool, sizeof(vs_pool_large_t));
    if (large == NULL) {
		vs_log_sys_error("vs_palloc_small fail");
        vs_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;				//头插法
	
	pool->large_nums++;
    return p;
}

void * vs_palloc_just_large(vs_pool_t *pool, size_t size)
{
    return vs_palloc_large(pool, size);
}



vs_int_t
vs_pfree(vs_pool_t *pool, void *p)
{
    vs_pool_large_t  *l;

    for (l = pool->large; l; l = l->next) {
        if (p == l->alloc) {
            vs_free(l->alloc);
            l->alloc = NULL;
			pool->large_nums--;
            return VS_OK;
        }
    }
	
    return VS_DECLINED;
}




