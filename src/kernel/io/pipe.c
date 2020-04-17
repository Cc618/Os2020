#include "pipe.h"

#include <k/queue.h>

File *Pipe_new(fd_t fd)
{
    Queue *data = Queue_new(PIPE_CAPACITY);

    return File_new(fd, data, Pipe_ops());
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
