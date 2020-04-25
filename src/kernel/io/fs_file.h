#pragma once

// To have a FSEntry as a file

#include "file.h"

// Opens a file at this location
// * path is absolute
File *FSFile_new(const char *path);

FileOps *FSFile_ops();

ssize_t FSFile_read(File *f, void *buffer, size_t count);

// TMP : write
// ssize_t FSFile_write(File *f, void *buffer, size_t count);

void FSFile_close(File *f);
