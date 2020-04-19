#include "pipe.h"

#include <k/queue.h>

File *Pipe_new()
{
    Queue *data = Queue_new(PIPE_CAPACITY);

    File *pipe = File_new(data, Pipe_ops());

    registerFile(pipe);

    return pipe;
}

// Returns file ops for a pipe
FileOps *Pipe_ops()
{
    FileOps *ops = malloc(sizeof(FileOps));

    *ops = (FileOps) {
        .read = Pipe_read,
        .write = Pipe_write,
    };

    return ops;
}

void Pipe_del(File *f)
{
    Queue_del(f->data);

    deregisterFile(f->fd);

    File_del(f);
}

size_t Pipe_read(File *f, void *buffer, size_t count)
{
    Queue *q = f->data;
    size_t n = 0;
    for ( ; n < count; ++n)
    {
        ((u8*) buffer)[n] = Queue_pop(q);

        if (Queue_empty(q))
            break;
    }
    
    return ++n;
}

size_t Pipe_write(File *f, void *buffer, size_t count)
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
