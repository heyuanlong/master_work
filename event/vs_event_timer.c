#include "stdlib.h"

#include "vs_event_timer.h"
#include "vs_event.h"
#include "vs_net.h"
#include "vs_time.h"


vs_thread_volatile vs_rbtree_t  	vs_event_timer_rbtree;
static vs_rbtree_node_t          	vs_event_timer_sentinel;

int vs_event_timer_init()
{
	 vs_rbtree_init(&vs_event_timer_rbtree, &vs_event_timer_sentinel,vs_rbtree_insert_timer_value);
	 return VS_OK;
}

int vs_event_timer_add( vs_conn_t* conn,  int timeout )
{
	vs_rbtree_node_t					*node;
    vs_rbtree_key_t                     key;

    if (conn->is_set_timeout == 1){
    	vs_event_timer_del(conn);
    }
    timeout += vs_current_sec;
    conn->timer_node.key.v = timeout;
    conn->timer_node.key.v2 = 0;
    vs_rbtree_insert( &vs_event_timer_rbtree, &conn->timer_node);
    conn->is_set_timeout = 1;
    conn->timeout = timeout;

    return VS_OK;
}

int vs_event_timer_del( vs_conn_t* conn )
{
	if (conn->is_set_timeout){
		vs_rbtree_delete( &vs_event_timer_rbtree, &conn->timer_node );
	    conn->is_set_timeout = 0;
	    conn->timeout = 0;
	    return VS_OK;
	}
    return VS_ERROR;
}

void vs_event_timer_expire()
{
    vs_conn_t               *conn;
    vs_event_timer_t        *tev;
    vs_rbtree_node_t        *node, *root, *sentinel;

    sentinel = vs_event_timer_rbtree.sentinel;

    for ( ;; ) {
        root = vs_event_timer_rbtree.root;
        if (root == sentinel) {
            return;
        }

        node = vs_rbtree_min(root, sentinel);

        if ( (long long)(node->key.v - vs_current_sec) > 0) {
            //vs_log_sys_info("vs_event_timer_expire %d, %d",node->key.v , vs_current_sec);
            return;
        }

        conn = (vs_conn_t *) ((char *) node - offsetof(vs_conn_t, timer_node));
        tev = conn->tev;
        vs_rbtree_delete(&vs_event_timer_rbtree, &conn->timer_node);

        conn->is_set_timeout = 0;
        conn->timeout = 0;
        tev->handle(tev);
    }
}

