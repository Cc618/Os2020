#pragma once

// FAT32 file system driver

#include "fs/fs.h"
#include "k/types.h"

void fatInit();

void fatTerminate();

// The pointer data in FSEntry points to this structure
// Describes a Fat32 directory
typedef struct FatEntryData_t
{
    // Where the content is
    u32 cluster;
} FatEntryData;

FSEntry *fatFSEntry_new(const char *name, u8 flags, FatEntryData *data);

void fatFSEntry_del(FSEntry *entry);

void FatFSEntryData_del(FatEntryData *data);

FSEntry **fatEnumDir(FSEntry *dir);

// Generates the root entry
FSEntry *fatGenRoot();
