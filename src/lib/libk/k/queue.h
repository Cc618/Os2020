#pragma once

// A static FIFO
// If the user overflows the capacity, a fatal
// error is thrown

#include <k/types.h>

typedef struct Queue_t
{
    // Number of items in data
    size_t capacity;
    // Array of pointers to items
    void **data;
    // Indices of the first / (last + 1) items
    size_t start;
    size_t end;
} Queue;

Queue *Queue_new(size_t capacity);

// Frees all items
void Queue_del(Queue *q);

// Enqueue
void Queue_add(Queue *q, void *item);

// Dequeue
void *Queue_pop(Queue *q);

// Deletes all items
void *Queue_clear(Queue *q);

// For each
void *Queue_iter(Queue *q, void (*functor)(void *item));

static inline bool Queue_empty(const Queue *q)
{ return q->start == q->end; }
