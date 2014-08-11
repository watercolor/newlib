#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* next;
}node_t;

node_t *list_insert_sort(node_t *list)
{
    node_t *next_cmp, *newhead;
    if (NULL == list) {
        return NULL;
    }
    next_cmp = list->next;
    newhead = list;
    list->next = NULL;
    while(next_cmp != NULL) {
        node_t **walk_node = &newhead;
        node_t *last_node;
        node_t *cmp_node = next_cmp;

        next_cmp = next_cmp->next; /* point to next need compare node */
        while(NULL != *walk_node) {
            if(cmp_node->data > (*walk_node)->data) {
                last_node = *walk_node;
                walk_node = &(*walk_node)->next;
            } else {
                /* insert head or insert middle reach this branch */
                cmp_node->next = *walk_node;
                *walk_node = cmp_node;
                break;
            }
        }
        /* new node need add tail reach this branch */
        if(NULL == *walk_node) {
            last_node->next = cmp_node;
            cmp_node->next = NULL;
        }
    }
    return newhead;
}

int main(int argc, const char *argv[])
{
    node_t array[] = {
        {2, &array[1]},
        {4, &array[2]},
        {1, &array[3]},
        {3, &array[4]},
        {5, NULL},
    };
    node_t *newhead, *node;
    newhead = list_insert_sort(array);

    node = newhead;
    while(node) {
        printf("%d\n", node->data);
        node = node->next;
    }
    return 0;
}
