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

FSEntry *FSEntry_new(const char *name, u8 flags, void *data)
{
    FSEntry *entry = malloc(sizeof(FSEntry));

    entry->name = name;
    entry->flags = flags;
    entry->data = data;

    return entry;
}

void FSEntry_del(FSEntry *entry)
{
    free(entry->name);
}

