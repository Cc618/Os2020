#pragma once

// Gathers linearly a sequence of data
// Used also as a stack
// Frees items in del

#include <stddef.h>

typedef struct Vector
{
    // Number of items in data
    size_t capacity;
    // Number of valid items in data
    size_t size;
    // Array of pointers to items
    void **data;
} Vector;

Vector *Vector_new();

// Returns the item at the index i
void *Vector_at(Vector *v, size_t i);

// Adds an entry
void Vector_add(Vector *v, void *item);

// Removes the last entry and returns it
void *Vector_pop(Vector *v);

// Removes and frees all items
// The capacity remains inchanged
void Vector_clear(Vector *v);

void Vector_del(Vector *v);

// For each
void Vector_iter(Vector *v, void (*functor)(void *item));
