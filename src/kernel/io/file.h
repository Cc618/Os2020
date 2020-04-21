#pragma once

// A virtual file, can be for example a pipe

#include <k/types.h>

// --- File --- //
struct FileOps_t;

typedef struct File_t
{
    // Descriptor
    fd_t fd;
    void *data;
    struct FileOps_t *ops;
} File;

// Operations in a file
typedef struct FileOps_t
{
    // See File_read
    // If NULL, the file is not readable
    size_t (*read)(File *f, void *buffer, size_t count);

    // See File_write
    // If NULL, the file is not writable
    size_t (*write)(File *f, void *buffer, size_t count);

    // Like a destructor
    void (*close)(File *f);
} FileOps;

// Creates and register the file
File *File_new(void *data, FileOps *ops);

// Deregister and closes the file
void File_del(File *f);

// Reads count bytes in buffer
// Returns how many bytes read
size_t File_read(File *f, void *buffer, size_t count);

// Writes count bytes of buffer
// Returns how many bytes written
size_t File_write(File *f, void *buffer, size_t count);

// Like a destructor
void File_close(File *f);

// --- Files --- //
void filesInit();

void filesTerminate();

// Adds the file to the file vector
// to allow file access via the file descriptor
void registerFile(File *f);

// Removes the access of the file associated
// to this fd
// * f->fd is set to -1 if found
void deregisterFile(File *f);

// Retrieve a file with this fd
// !!! Returns NULL on negative search
File *getFile(fd_t fd);
