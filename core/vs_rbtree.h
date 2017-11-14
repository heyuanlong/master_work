#ifndef __VS_RBTREE_H__
#define __VS_RBTREE_H__



#define vs_thread_volatile   volatile
#ifdef WIN32
#define vs_inline __inline
#else
#define vs_inline inline
#endif

#include "vs_core.h"


typedef struct vs_rbtree_s          vs_rbtree_t;
typedef struct vs_rbtree_node_s     vs_rbtree_node_t;


typedef struct vs_rbtree_key_s      vs_rbtree_key_t;
typedef int                        vs_rbtree_key_int_t;
typedef  int                       vs_int32_t;

struct vs_rbtree_key_s
{
    unsigned long long     v;
    unsigned long long     v2;
};

struct vs_rbtree_node_s
{
    vs_rbtree_key_t        key;
    vs_rbtree_node_t	  *left;
    vs_rbtree_node_t      *right;
    vs_rbtree_node_t      *parent;
    unsigned char          color;
    unsigned char          data;
};

typedef void (*vs_rbtree_insert_pt) (vs_rbtree_node_t *root,
    vs_rbtree_node_t *node, vs_rbtree_node_t *sentinel);

typedef int (*vs_rbtree_compare_pt)(vs_rbtree_key_t *p1, vs_rbtree_key_t *p2);

#define vs_RBTREE_TYPE_DEFAULT_NUMBER                   0
#define vs_RBTREE_TYPE_STR                              1

struct vs_rbtree_s {
    vs_rbtree_node_t     *root;
    vs_rbtree_node_t     *sentinel;
    vs_rbtree_insert_pt   insert;
    vs_rbtree_compare_pt  compare;
    vs_int32_t            rbtree_type;
};

#define vs_rbtree_init(tree, s, i)                                           \
    vs_rbtree_sentinel_init(s);                                              \
    (tree)->root = s;                                                         \
    (tree)->sentinel = s;                                                     \
    (tree)->compare = 0;                                                      \
    (tree)->insert = i                                                  

#define vs_rbtree_init_with_compare(tree,s,i,c)                                \
    vs_rbtree_sentinel_init(s);                                                \
    (tree)->root = s;                                                          \
    (tree)->sentinel = s;                                                      \
    (tree)->compare = c;                                                       \
    (tree)->insert = i                                                  

void vs_rbtree_insert(vs_thread_volatile vs_rbtree_t *tree,
    vs_rbtree_node_t *node);
void vs_rbtree_delete(vs_thread_volatile vs_rbtree_t *tree,
    vs_rbtree_node_t *node);
int vs_rbtree_get(vs_thread_volatile vs_rbtree_t *tree, vs_rbtree_key_t  key,
	vs_rbtree_node_t *sentinel, vs_rbtree_node_t **node);



//vs_rbtree_insert_value 和 vs_rbtree_insert_timer_value  里面的实现是一样的。
void vs_rbtree_insert_value(vs_rbtree_node_t *root, vs_rbtree_node_t *node,
    vs_rbtree_node_t *sentinel);
void vs_rbtree_insert_timer_value(vs_rbtree_node_t *root,
    vs_rbtree_node_t *node, vs_rbtree_node_t *sentinel);


#define vs_rbt_red(node)               ((node)->color = 1)
#define vs_rbt_black(node)             ((node)->color = 0)
#define vs_rbt_is_red(node)            ((node)->color)
#define vs_rbt_is_black(node)          (!vs_rbt_is_red(node))
#define vs_rbt_copy_color(n1, n2)      (n1->color = n2->color)

#define vs_rbtree_sentinel_init(node)  vs_rbt_black(node)

static vs_inline vs_rbtree_node_t*
vs_rbtree_min(vs_rbtree_node_t *node, vs_rbtree_node_t *sentinel)
{
    while (node->left != sentinel) {
        node = node->left;
    }

    return node;
}

static vs_inline vs_rbtree_node_t*
vs_rbtree_max(vs_rbtree_node_t *node, vs_rbtree_node_t *sentinel)
{
	while (node->right != sentinel) {
		node = node->right;
	}

	return node;
}


#define offsetof(type, member) (size_t)&(((type*)0)->member)
#define container_of(ptr, type, member) ({                      \
         const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})



#endif //
