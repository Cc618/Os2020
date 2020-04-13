#include "fs.h"

#include "drivers/fat32.h"

FSEntry *root;

void fsInit()
{
    root = fatGenRoot();
}

void fsTerminate()
{
    // TODO : Call delete method (free name...)
    free(root);
}

// FSEntry methods //
FSEntry *FSEntry_new(const char *name, u8 flags, size_t size, void *data, FSEntryOps *ops)
{
    FSEntry *entry = malloc(sizeof(FSEntry));

    *entry = (FSEntry){
        .name = name,
        .flags = flags,
        .size = size,
        .data = data,
        .ops = ops
    };

    return entry;
}

size_t FSEntry_read(FSEntry *entry, void *buffer, size_t count)
{
    // Not a file
    if (entry->flags & FS_DIRECTORY)
        return 0;
    
    return entry->ops->read(entry, buffer, count);
}

FSEntry **FSEntry_list(FSEntry *dir)
{
    // Not a directory
    if (!(dir->flags & FS_DIRECTORY))
        return NULL;
    
    return dir->ops->list(dir);
}

void FSEntry_del(FSEntry *entry)
{
    free(entry->name);
    free(entry);
}

