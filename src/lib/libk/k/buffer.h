#pragma once

// A buffer is like a vector but the data describes bytes and not pointers

#include "types.h"

typedef struct Buffer_t
{
    size_t size;
    // Next byte to read / write
    size_t head;
    // Capacity in bytes
    size_t capacity;
    // When we have to reallocate, we increase the capacity
    // by a chunk (or more depending on how much data we write)
    size_t chunkSize;
    u8 *data;
} Buffer;

Buffer *Buffer_new(size_t chunkSize);

// * Head is set to 0
Buffer *Buffer_newFromBuffer(size_t chunkSize, void *buffer, size_t count);

void Buffer_del(Buffer *buf);

// Returns how many bytes read
size_t Buffer_read(Buffer *buf, void *data, size_t count);

// Returns how many bytes written
size_t Buffer_write(Buffer *buf, void *data, size_t count);

// Reallocate the buffer, the content is preserved
// - Chunks are the number of chunks we allocate (not add)
void Buffer_realloc(Buffer *buf, size_t chunks);
