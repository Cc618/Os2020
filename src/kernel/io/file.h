#pragma once

// A virtual file, can be for example a pipe

#include <k/types.h>

// File descriptor type
typedef u32 fd_t;

// Operations in a file
typedef struct FileOps_t
{
    // See File_read
    // If NULL, the file is not readable
    size_t (*read)(File *f, void *buffer, size_t count);

    // See File_write
    // If NULL, the file is not writable
    size_t (*write)(File *f, void *buffer, size_t count);
} FileOps;

typedef struct File_t
{
    // Descriptor
    fd_t fd;
    void *data;
    FileOps *ops;
} File;

File *File_new(fd_t fd, void *data, FileOps *ops);

// !!! Doesn't free data
void File_del(File *f);

// Reads count bytes in buffer
// Returns how many bytes read
size_t File_read(File *f, void *buffer, size_t count);

// Writes count bytes of buffer
// Returns how many bytes written
size_t File_write(File *f, void *buffer, size_t count);
