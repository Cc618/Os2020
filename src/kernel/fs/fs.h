#pragma once

// The file system

#include "k/types.h"

#define FS_DIRECTORY    0x1
#define FS_HIDDEN       0x2

struct FSEntryOps_t;

// Describes a file, directory, root...
// * Object
typedef struct FSEntry_t
{
    // Name, not the path
    char *name;
    u8 flags;

    // Used by drivers
    void *data;

    // Methods
    struct FSEntryOps_t *ops;
} FSEntry;

// All methods of FSEntry
typedef struct FSEntryOps_t
{
    // Free
    void (*del)(FSEntry *entry);

    // Returns a null terminated
    // array of entries within this
    // directory, returns NULL if
    // not a directory 
    FSEntry **(*list)(FSEntry *entry);
} FSEntryOps;

extern FSEntry *root;

// !!! FS drivers MUST be init before
void fsInit();

// !!! FS drivers MUST be terminated after
void fsTerminate();

FSEntry *FSEntry_new(const char *name, u8 flags, void *data, FSEntryOps *ops);

// !!! Doesn't frees data
void FSEntry_del(FSEntry *entry);

// TMP :
#include <stdio.h>
static inline void printEntry(FSEntry *entry)
{
    printf("- Entry :\n");
    printf("Name         %s\n", entry->name);
    printf("Directory    %s\n", entry->flags & FS_DIRECTORY ? "true" : "false");
    printf("Hidden       %s\n", entry->flags & FS_HIDDEN ? "true" : "false");
    // printf("Data         %x\n", *(u32*)entry->data);
}
