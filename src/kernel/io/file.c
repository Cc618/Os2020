#include "file.h"

#include <stdlib.h>

File *File_new(fd_t fd, void *data, FileOps *ops)
{
    File *f = malloc(sizeof(File));

    f->fd = fd;
    f->data = data;
    f->ops = ops;

    return f;
}

// !!! Doesn't free data
void File_del(File *f)
{
    free(f->ops);
    free(f);
}

// Reads count bytes in buffer
// Returns how many bytes read
size_t File_read(File *f, void *buffer, size_t count)
{
    // Not readable
    if (f->ops->read == NULL)
        return 0;

    return f->ops->read(f, buffer, count);
}

// Writes count bytes of buffer
// Returns how many bytes written
size_t File_write(File *f, void *buffer, size_t count)
{
    // Not writable
    if (f->ops->write == NULL)
        return 0;

    return f->ops->write(f, buffer, count);
}
