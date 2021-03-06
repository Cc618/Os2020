#pragma once

// The virtual file system

#include "k/types.h"

#define FS_DIRECTORY    0x1
#define FS_HIDDEN       0x2

struct FSEntryOps_t;

// Describes a file, directory, root...
// * Object
typedef struct FSEntry_t
{
    // Name, not the path
    const char *name;
    u8 flags;
    // 0 if directory, the size in bytes of the file
    size_t size;

    // Used by drivers
    void *data;

    // Methods
    struct FSEntryOps_t *ops;
} FSEntry;

// All methods of FSEntry
typedef struct FSEntryOps_t
{
    // Free data
    void (*del)(FSEntry *entry);

    // See FSEntry_read for details
    // * The entry is always a file
    size_t (*read)(FSEntry *entry, void *buffer, size_t count);

    // See FSEntry_write for details
    // * The entry is always a file
    size_t (*write)(FSEntry *entry, void *buffer, size_t count);

    // See FSEntry_list for details
    // * The entry is always a directory
    FSEntry **(*list)(FSEntry *entry, size_t *outCount);

    // See FSEntry_touch for details
    // * The entry is always a directory
    FSEntry *(*touch)(FSEntry *entry, const char *name, u8 flags);
} FSEntryOps;

extern FSEntry *root;

// !!! FS drivers MUST be init before
void fsInit();

// !!! FS drivers MUST be terminated after
void fsTerminate();

// FSEntry methods
FSEntry *FSEntry_new(const char *name, u8 flags, size_t size, void *data, FSEntryOps *ops);

// Reads count bytes of entry in buffer
// Returns how many bytes read
// * If entry is a directory, returns 0 
size_t FSEntry_read(FSEntry *entry, void *buffer, size_t count);

// Writes count bytes of buffer in entry
// Returns how many bytes written
// * Overwrites the data
// * If entry is a directory, returns 0 
size_t FSEntry_write(FSEntry *entry, void *buffer, size_t count);

// Returns an
// array of entries within this
// directory, returns NULL if
// not a directory
// - outCount is set to the number of items
FSEntry **FSEntry_list(FSEntry *dir, size_t *outCount);

// Creates a new file / directory
// Returns NULL if not a directory
FSEntry *FSEntry_touch(FSEntry *entry, const char *name, u8 flags);

// !!! Doesn't frees data
void FSEntry_del(FSEntry *entry);

// Deletes / frees all entries and also the array entries
void delEntries(FSEntry **entries, size_t count);

// Finds an entry in the entry list with its name
// Returns NULL if not found
FSEntry *findEntry(FSEntry **entries, const char *name, size_t count);

// Retrieve an entry
// We can use \ or /
// Handle relative / absolute paths
// Returns NULL if not found
FSEntry *getEntry(const char *path);

