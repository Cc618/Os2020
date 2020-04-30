#pragma once

// To have a FSEntry as a file

#include "fs/fs.h"
#include "file.h"
#include <k/buffer.h>

typedef struct FSFileData_t
{
    FSEntry *file;

    // To write data we write to a buf
    // and when close is called we overwrite
    // the data of the file by the data in buf
    Buffer *buf;
} FSFileData;

// Opens a file at this location
// * path is absolute
File *FSFile_new(const char *path);

FileOps *FSFile_ops();

ssize_t FSFile_read(File *f, void *buffer, size_t count);

ssize_t FSFile_write(File *f, void *buffer, size_t count);

void FSFile_close(File *f);
