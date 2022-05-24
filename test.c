#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "pqueue.h"

int main(void) 
{
    int err = 0;
    struct pqueue *queue = NULL;
    uint64_t size = 5, *temp = 0;
    uint64_t test_values[] = {1, 2, 3, 4, 5};
    uint64_t test_priorities[] = {5, 4, 3, 2, 1};

    puts("testing pqueue");
    
    queue = pqueue_create(size);
    if (!queue) {
        puts("failed to create pqueue!");
        goto out;
    }

    for (int i = 0; i < 5; ++i) {
        err = pqueue_insert(queue, test_values + i, test_priorities[i]);
        if (err) {
            printf("err: %d. Failed to insert into queue!\n", err);
            goto put_queue;
        }
    }

    while(pqueue_size(queue)) {
        temp = pqueue_pop(queue);
        if (!temp) {
            puts("Value was not found!");
            goto put_queue;
        }
        printf("value: %lu\n", *temp);
    }

    pqueue_destroy(queue);
    return err;

put_queue:
    free(queue);
out:
    return err;
}
