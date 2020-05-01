#include "buffer.h"

#include <stdlib.h>
#include <string.h>

Buffer *Buffer_new(size_t chunkSize)
{
    Buffer *b = malloc(sizeof(Buffer));

    *b = (Buffer) {
        .size = 0,
        .head = 0,
        .capacity = chunkSize,
        .chunkSize = chunkSize,
        .data = malloc(chunkSize)
    };

    return b;
}

Buffer *Buffer_newFromBuffer(size_t chunkSize, void *buffer, size_t count)
{
    Buffer *b = malloc(sizeof(Buffer));

    size_t capacity = (count / chunkSize + 1) * chunkSize;

    *b = (Buffer) {
        .size = count,
        .head = 0,
        .capacity = capacity,
        .chunkSize = chunkSize,
        .data = malloc(capacity)
    };

    memcpy(b->data, buffer, count);

    return b;
}

void Buffer_del(Buffer *buf)
{
    free(buf->data);
    free(buf);
}

size_t Buffer_read(Buffer *buf, void *data, size_t count)
{
    // End of reading
    size_t readEnd = buf->head + count;
    if (readEnd > buf->size)
        readEnd = buf->size;

    // Read
    size_t len = readEnd - buf->head;
    memcpy(data, &buf->data[buf->head], len);

    buf->head = readEnd;

    return len;
}

size_t Buffer_write(Buffer *buf, void *data, size_t count)
{
    size_t writeEnd = buf->head + count;

    // Not enough place to write, reallocate
    if (writeEnd >= buf->capacity)
        Buffer_realloc(buf, writeEnd / buf->chunkSize + 1);

    // Copy the data
    memcpy(buf->data + buf->head, data, count);

    buf->head += count;

    if (buf->head > buf->size)
        buf->size = buf->head;

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
