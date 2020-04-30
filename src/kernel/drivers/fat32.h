#pragma once

// FAT32 file system driver

#include "fs/fs.h"
#include "k/types.h"

void fatInit();

void fatTerminate();

// The pointer data in FSEntry points to this structure
// Describes a Fat32 directory
typedef struct FatFSEntryData_t
{
    // Where the content is
    u32 cluster;

    // Where the directory cluster is
    u32 entryCluster;

    // Index within the directory (last entry for LFN, where there are flags)
    u32 entryI;
} FatFSEntryData;

FSEntry *fatFSEntry_new(const char *name, u8 flags, size_t size, FatFSEntryData *data);

void fatFSEntry_del(FSEntry *entry);

void FatFSEntryData_del(FatFSEntryData *data);

size_t fatFSEntry_read(FSEntry *file, void *buffer, size_t count);

size_t fatFSEntry_write(FSEntry *file, void *buffer, size_t count);

FSEntry **fatFSEntry_list(FSEntry *dir);

FSEntry *fatFSEntry_touch(FSEntry *dir, const char *name, u8 flags);

// Generates the root entry
FSEntry *fatGenRoot();

// Returns the ops structure with
// all methods of fat
FSEntryOps *fatGenFSEntryOps();
