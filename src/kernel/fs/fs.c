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
    free((void*) entry->name);
    free(entry);
}

void delEntries(FSEntry **entries)
{
    for (size_t i = 0; entries[i]; ++i)
        FSEntry_del(entries[i]);
    
    free(entries);
}

FSEntry *findEntry(FSEntry **entries, const char *name)
{
    for (size_t i = 0; entries[i]; ++i)
        if (strcmp(name, entries[i]->name) == 0)
            return entries[i];

    return NULL;
}

FSEntry *getEntry(const char *path)
{
    // Don't parse root
    if (path[0] == '/' || path[0] == '\\')
        ++path;

    char *p = strdup(path);
    const char *delim = "/\\";
    char *part = strtok(p, delim);
    
    // Current directory we parse
    const FSEntry *current = root;

    do
    {
        // ls
        FSEntry **entries = FSEntry_list(current);

        if (current != root)
            FSEntry_del(current);

        if (entries == NULL)
            return NULL;

        current = findEntry(entries, part);

        // Free all entries excluding current
        for (size_t i = 0; entries[i]; ++i)
            // Don't delete the first directory
            if (current == root || entries[i] != current)
                FSEntry_del(entries[i]);
        
        free(entries);

        if (current == NULL)
            return NULL;
    } while ((part = strtok(NULL, delim)));

    free(p);

    return current;
}
