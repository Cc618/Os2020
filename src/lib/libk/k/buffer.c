#include "buffer.h"

#include <stdlib.h>
#include <string.h>

Buffer *Buffer_new(size_t chunkSize)
{
    Buffer *b = malloc(sizeof(Buffer));

    *b = (Buffer) {
        .size = 0,
        .capacity = chunkSize,
        .chunkSize = chunkSize,
        .data = malloc(chunkSize)
    };

    return b;
}

void Buffer_del(Buffer *buf)
{
    free(buf->data);
    free(buf);
}

size_t Buffer_write(Buffer *buf, void *data, size_t count)
{
    // Not enough place to write, reallocate
    if (buf->size + count >= buf->capacity)
        Buffer_realloc(buf, (buf->size + count) / buf->chunkSize + 1);
    
    // Copy the data
    memcpy(buf->data + buf->size, data, count);

    buf->size += count;

    return count;
}

void Buffer_realloc(Buffer *buf, size_t chunks)
{
    buf->capacity = chunks * buf->chunkSize;

    // Duplicate data
    u8 *newData = malloc(chunks * buf->chunkSize);
    memcpy(newData, buf->data, buf->size);

    // Replace data
    free(buf->data);
    buf->data = newData;
}
