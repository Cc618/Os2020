#include "queue.h"

#include <k/syscalls.h>
#include <stdlib.h>

// To avoid to free empty items
static void tryFree(void *data)
{
    if (data == NULL)
        return;
    
    free(data);
}

Queue *Queue_new(size_t capacity)
{
    Queue *q = malloc(sizeof(Queue));
    q->capacity = capacity;
    q->data = malloc(capacity * sizeof(void*));
    q->start = q->end = 0;

    return q;
}

void Queue_del(Queue *q)
{
    Queue_iter(q, tryFree);
    
    free(q);
}

void Queue_add(Queue *q, void *item)
{
    q->data[q->end] = item;

    q->end = (q->end + 1) % q->capacity;

    if (Queue_full(q))
        fatal("Queue : Can't enqueue with fully filled queue");
}

void *Queue_pop(Queue *q)
{
    if (Queue_empty(q))
        fatal("Queue : Can't dequeue with empty queue");

    void *ret = q->data[q->start];

    q->start = (q->start + 1) % q->capacity;

    return ret;
}

void Queue_clear(Queue *q)
{
    Queue_iter(q, free);

    q->start = q->end = 0;
}

void Queue_iter(Queue *q, void (*functor)(void *item))
{
    for (size_t i = q->start; i != q->end; i = (i + 1) % q->capacity)
        functor(q->data[i]);
}

