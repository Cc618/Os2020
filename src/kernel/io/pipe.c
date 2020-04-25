#include "pipe.h"

#include <k/queue.h>

File *Pipe_new()
{
    Queue *data = Queue_new(PIPE_CAPACITY);

    File *pipe = File_new(data, Pipe_ops());

    return pipe;
}

// Returns file ops for a pipe
FileOps *Pipe_ops()
{
    FileOps *ops = malloc(sizeof(FileOps));

    *ops = (FileOps) {
        .read = Pipe_read,
        .write = Pipe_write,
        .close = Pipe_close,
    };

    return ops;
}

ssize_t Pipe_read(File *f, void *buffer, size_t count)
{
    Queue *q = f->data;

    if (Queue_empty(q))
        return 0;

    for (size_t n = 0; ; )
    {
        ((u8*) buffer)[n] = (u8)Queue_pop(q);

        ++n;

        if (n >= count)
            return n;

        if (Queue_empty(q))
            return n;
    }
}

ssize_t Pipe_write(File *f, void *buffer, size_t count)
{
    size_t n = 0;
    Queue *q = f->data;
    for ( ; n < count; ++n)
    {
        Queue_add(q, ((u8*) buffer)[n]);

        if (Queue_full(q))
            break;
    }

    return n;
}

void Pipe_close(File *f)
{
    Queue_del(f->data);
}
