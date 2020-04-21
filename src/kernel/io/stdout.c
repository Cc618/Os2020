#include "stdout.h"

#include "drivers/console.h"
#include <stdlib.h>

void Stdout_init()
{
    FileOps *ops = malloc(sizeof(FileOps));
    ops->read = NULL;
    ops->write = Stdout_write;
    ops->close = NULL;
    
    File_new(NULL, ops);
}

size_t Stdout_write(File *f, void *buffer, size_t count)
{
    for (size_t i = 0; i < count; ++i)
        consolePut(((char*) buffer)[i]);
    
    return count;
}
