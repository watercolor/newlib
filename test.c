#include <stdio.h>
#include "lf_list.h"

int main(int argc, const char *argv[])
{
    int i;
    int array[10] = {1,2,3,4,5,6,7,8,9, 10};
    lf_listhead list = INIT_LFLIST_HEAD(list);
    /* lf_list_init(&list); */
    EnQueue(&list, array[0]);
    EnQueue(&list, array[1]);
    EnQueue(&list, array[2]);
    EnQueue(&list, array[3]);
    EnQueue(&list, array[4]);
    EnQueue(&list, array[5]);
    EnQueue(&list, array[6]);
    EnQueue(&list, array[7]);
    EnQueue(&list, array[8]);
    EnQueue(&list, array[9]);
    for (i = 0; i < 10; i++) {
        int value = DeQueue(&list, int);
        if(!IS_QUEUE_EMPTY(value)) {
            printf("%d\n", value);
        }
    }
    return 0;
}
