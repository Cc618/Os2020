#pragma once

// FInfo is a structure that gathers properties of files
// Use the finfo syscall to obtain a FInfo

#include <k/types.h>

typedef struct FInfo_t
{
    size_t size;
    bool directory;
} FInfo;
