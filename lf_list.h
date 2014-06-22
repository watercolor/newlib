#ifndef __LF_LIST_H__
#define __LF_LIST_H__


#include <stdlib.h>
enum {
    QUEUE_FALSE,
    QUEUE_TRUE,
};

#define QUEUE_EMPTY ((void*)(long)-1)

#define IS_QUEUE_EMPTY(value) ((void*)(long)(value) == QUEUE_EMPTY)

#define CAS(ptr, oldval, newval) \
    __sync_bool_compare_and_swap(ptr, oldval, newval)

typedef struct list_node{
    struct list_node *pstNext;
    void * value;
}lf_listnode;


typedef struct lf_listhead{
    lf_listnode *head;
    lf_listnode *tail;
    lf_listnode dummy;
}lf_listhead;

#define INIT_LFLIST_HEAD(name)  {&(name.dummy), &(name.dummy), {NULL, NULL}}

static inline void lf_list_init(lf_listhead *lf_list)
{
    lf_list->dummy.pstNext = NULL;
    lf_list->tail = &lf_list->dummy;
    lf_list->head = &lf_list->dummy;
}
/**
 * queue_offsetof - get offset of member in type
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 */
#define queue_offsetof(type, member) ((size_t) &((type *)0)->member)

/**
 * queue_container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define queue_container_of(ptr, type, member) ({                      \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
	(type *)( (char *)__mptr - queue_offsetof(type,member) );})

#define lf_list_entry(ptr, type, member)		\
	queue_container_of(ptr, type, member)

static inline lf_listnode * NewNode(void* element)
{
    lf_listnode* pstnode = malloc(sizeof(lf_listnode));
    if(pstnode) {
        pstnode->pstNext = NULL;
        pstnode->value = element;
    }
    return pstnode;
}

#define EnQueue(plisthead, element)  _EnQueue(plisthead, (void*)element)

static inline void _EnQueue(lf_listhead *lf_list, void *element)
{
    lf_listnode *last;
    int success;
    lf_listnode *newnode = NewNode(element);

    do {
        last = lf_list->tail;
        success = CAS(&last->pstNext, NULL, newnode);
        if(success != QUEUE_TRUE)
            CAS(&lf_list->tail, last, last->pstNext);
    } while (success != QUEUE_TRUE);
    CAS(&lf_list->tail, last, newnode);
}

#define DeQueue(plisthead, type)  ((type)_DeQueue(plisthead))

static inline void *_DeQueue(lf_listhead *lf_list)
{
    lf_listnode *head;
    do {
        head = lf_list->head;
        if (head->pstNext == NULL) {
            return QUEUE_EMPTY;
        }
    } while (CAS(&lf_list->head, head, head->pstNext) != QUEUE_TRUE);
    return head->pstNext->value;
}

#endif
