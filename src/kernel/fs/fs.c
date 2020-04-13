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

void FSEntry_del(FSEntry *entry)
{
    free(entry->name);
    free(entry);
}

