#include "file.h"

#include "stdin.h"
#include "stdout.h"
#include "stderr.h"
#include <k/vector.h>
#include <stdlib.h>

// --- File --- //
File *File_new(void *data, FileOps *ops)
{
    File *f = malloc(sizeof(File));

    f->data = data;
    f->ops = ops;

    registerFile(f);

    return f;
}

void File_del(File *f)
{
    deregisterFile(f);

    free(f->ops);
    free(f);
}

// Reads count bytes in buffer
// Returns how many bytes read
ssize_t File_read(File *f, void *buffer, size_t count)
{
    // Not readable
    if (f->ops->read == NULL)
        return -1;

    return f->ops->read(f, buffer, count);
}

// Writes count bytes of buffer
// Returns how many bytes written
ssize_t File_write(File *f, void *buffer, size_t count)
{
    // Not writable
    if (f->ops->write == NULL)
        return -1;

    return f->ops->write(f, buffer, count);
}

void File_close(File *f)
{
    // No need to close
    if (f->ops->close != NULL)
        f->ops->close(f);

    File_del(f);
}

// --- Files --- //
// Gathers all files accessible with file descriptors
static Vector *files;

void filesInit()
{
    files = Vector_new();

    Stdin_init();
    Stdout_init();
    Stderr_init();
}

void filesTerminate()
{
    // Custom destructor
    for (size_t i = 0; i < files->size; ++i)
        if (files->data[i] != NULL)
            File_close((File*) files->data[i]);

    free(files);
}

void registerFile(File *f)
{
    f->fd = -1;

    // Try to find NULL entry
    for (size_t i = 3; i < files->size; ++i)
        if (files->data[i] == NULL)
        {
            f->fd = i;
            files->data[i] = f;
            break;
        }

    if (f->fd == -1)
    {
        f->fd = files->size;

        Vector_add(files, f);
    }
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
