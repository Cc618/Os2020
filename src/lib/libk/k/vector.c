#include "vector.h"

#include <k/syscalls.h>
#include <stdlib.h>
#include <string.h>

// Number of items to allocate when
// the Vector runs out of size
#define VECTOR_CAPACITY_DELTA 32

Vector *Vector_new()
{
    Vector *v = malloc(sizeof(Vector));
    v->capacity = 32;
    v->size = 0;
    v->data = malloc(sizeof(void*) * v->capacity);

    return v;
}

void *Vector_at(Vector *v, size_t i)
{
    // TODO : Error, index out of bounds
    if (i >= v->size)
        return NULL;

    return v->data[i];
}

void Vector_add(Vector *v, void *item)
{
    ++v->size;

    // Check if there is enough capacity
    if (v->size >= v->capacity)
    {
        // Reallocate data
        size_t newCapacity = v->capacity + VECTOR_CAPACITY_DELTA;
        void **newData = malloc(sizeof(void*) * newCapacity);
        
        // Copy data
        memcpy(newData, v->data, sizeof(void*) * v->capacity);
        free(v->data);

        // Update attributes
        v->capacity = newCapacity;
        v->data = newData;
    }

    // Add the item
    v->data[v->size - 1] = item;
}

void *Vector_pop(Vector *v)
{
    if (v->size == 0)
        fatal("Tried to Vector_pop with an empty vector");
    
    --v->size;

    // Intended overflow
    return v->data[v->size];
}

void Vector_clear(Vector *v)
{
    for (size_t i = 0; i < v->size; ++i)
        free(v->data[i]);
    
    v->size = 0;
}

void Vector_del(Vector *v)
{
    // Free items
    Vector_clear(v);

    // Free array
    free(v->data);
}

void Vector_iter(Vector *v, void (*functor)(void *item))
{
    for (size_t i = 0; i < v->size; ++i)
        functor(v->data[i]);
}
