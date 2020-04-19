#include "file.h"

#include <k/vector.h>
#include <stdlib.h>

// --- File --- //
File *File_new(void *data, FileOps *ops)
{
    File *f = malloc(sizeof(File));

    f->fd = -1;
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

// --- Files --- //
// Gathers all files accessible with file descriptors
static Vector *files;

void filesInit()
{
    files = Vector_new();

    // TODO : Init first files (stdin...)
}

void filesTerminate()
{
    // Custom destructor
    // TODO : Delete files here ?
    Vector_iter(files, File_del);

    free(files);
}

void registerFile(File *f)
{
    // TODO : Find NULL entry

    f->fd = files->size;

    Vector_add(files, f);
}

void deregisterFile(File *f)
{
    // Exists
    if (f->fd < files->size)
    {
        files->data[f->fd] = NULL;
        f->fd = -1;
    }
}

File *getFile(fd_t fd)
{
    // May be found (can be also NULL)
    if (fd < files->size)
        return files->data[fd];
    
    return NULL;
}
