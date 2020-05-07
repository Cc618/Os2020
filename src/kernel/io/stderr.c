#include "stderr.h"

#include "drivers/console.h"
#include "drivers/screen.h"
#include <stdlib.h>

void Stderr_init()
{
    FileOps *ops = malloc(sizeof(FileOps));
    ops->read = NULL;
    ops->write = Stderr_write;
    ops->close = NULL;
    
    File_new(NULL, ops);
}

ssize_t Stderr_write(__attribute__((unused)) File *f, void *buffer, size_t count)
{
    // Push format
    u16 consoleFmtOld = consoleFmt;

    consoleFmt = FMT_ERROR;

    for (size_t i = 0; i < count; ++i)
        consolePut(((char*) buffer)[i]);
    
    // Pop format
    consoleFmt = consoleFmtOld;

    return count;
}
