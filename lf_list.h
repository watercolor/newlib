#include <stdio.h>
#include <unistd.h>
enum {
    FALSE,
    TRUE,
};
#define CAS(ptr, oldval, newval) \
    __sync_bool_compare_and_swap(ptr, oldval, newval)

typedef struct list_node{
    struct list_node *pstNext;
}lf_listnode;


typedef struct lf_listhead{
    lf_listnode *head;
    lf_listnode *tail;
}lf_listhead;

static inline void lf_list_init(lf_listhead *lf_list)
{
    lf_list->tail = NULL;
    // lf_list->head = &lf_list->tail;
    lf_list->head = NULL;
}

// #define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({                      \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
	(type *)( (char *)__mptr - offsetof(type,member) );})

#define lf_list_entry(ptr, type, member)		\
	container_of(ptr, type, member)

static inline void EnQueue(lf_listhead *lf_list, lf_listnode *element)
{
    lf_listnode *last;
    do {
        last = lf_list->tail;
        if(last == NULL) {
            /*first element, set tail, set head, if false, refetch tail */
            if (CAS((long*)&lf_list->tail, last, element) == TRUE) {
                lf_list->head = element;
                return;
            }
        }
    } while (CAS(&last->pstNext, NULL, element) != TRUE);
    CAS(&lf_list->tail, last, element);
}


static inline lf_listnode *DeQueue(lf_listhead *lf_list)
{
    lf_listnode *head;
    do {
        head = lf_list->head;
        if (head == NULL) {
            return NULL;
        }
    } while (CAS(&lf_list->head, head, head->pstNext) != TRUE);
    return head;
}

#define EnQueue2(list_head, element) do{  \
    do { \
        typeof(element) *last = list_head->tail; \
    } while (CAS(last->pstNext, NULL, element) != TRUE); \
    CAS(list_head->tail, last, element);  \
}while(0)

#define DeQueue2(list_head, element) do{  \
    do {  \
        typeof(element) *head = list_head->head;  \
        if (head == &list_head->head) {  \
            return NULL;  \
        }  \
    } while (CAS(list_head->head, head, head->pstNext) != TRUE);  \
    return head;  \
}while(0)
