/*
 * pqueue.h
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/* Struct definitions */

struct pqueue_entry {
    int64_t priority;
    void *value;
};

struct pqueue {
    uint64_t capacity;
    uint64_t size;
    struct pqueue_entry heap[];
};

#define node_left(i) (2*(i)+1)
#define node_right(i) (2*(i)+2)

static inline void
heapify(struct pqueue *pq)
{
    uint64_t largest_idx = 0, idx = 0;
    struct pqueue_entry temp_entry, *heap = pq->heap;

again:
    /* check both the left and right if possible */
    if (node_left(idx) <= pq->size - 1 &&
            heap[node_left(idx)].priority > heap[idx].priority) {
        largest_idx = node_left(idx);
    }
    if (node_right(idx) <= pq->size - 1 &&
            heap[node_right(idx)].priority > heap[largest_idx].priority) {
        largest_idx = node_right(idx);
    }

    /* largest was not the start, exchange and recurse */
    if (largest_idx != idx) {
        temp_entry = heap[idx];
        heap[idx] = heap[largest_idx];
        heap[largest_idx] = temp_entry;

        /* not done yet, process the largest_idx next */
        idx = largest_idx;
        goto again;
    } 
}

/* Functions for manipulating priority queue */

static struct pqueue *
pqueue_create(size_t capacity)
{
    struct pqueue *pqueue = NULL;
    size_t alloc_size = 0;

    // an additional space is used for moving things
    alloc_size = sizeof(struct pqueue) +
        (capacity) * sizeof(struct pqueue_entry);
    pqueue = (struct pqueue *)malloc(alloc_size);

    if (!pqueue)
        return NULL;

    memset(pqueue, 0, alloc_size);

    pqueue->capacity = capacity;

    return pqueue;
}

static int
pqueue_insert(struct pqueue *pq, void *value, int64_t priority)
{
    int err = 0;
    
    /* fail if at capacity */
    if (pq->size == pq->capacity) {
        err = -ENOMEM;
        goto out;
    }

    /* shift the array over by one to allow for a new root */
    memmove(pq->heap + 1, pq->heap, pq->size * sizeof(struct pqueue_entry));

    pq->heap[0].priority = priority;
    pq->heap[0].value = value;

    pq->size++;

    heapify(pq);

out:
    return err;

}

static void *
pqueue_pop(struct pqueue *pq)
{
    void *value = NULL;

    /* check if the queue is empty */
    if (pq->size == 0) {
        puts("empty queue!");
        return NULL;
    }

    value = pq->heap[0].value;

    /* delete the value from the heap */
    memmove(pq->heap, pq->heap + 1, (pq->size - 1) *
            sizeof(struct pqueue_entry));

    /* fix the heap */
    heapify(pq);

    --pq->size;

    return value;
}

static void
pqueue_destroy(struct pqueue *pq)
{
    free(pq);
}

static size_t
pqueue_size(struct pqueue *pq)
{
    return pq->size;
}
