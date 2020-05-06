#pragma once

// A context is a struct describing a running app

#include "types.h"

typedef struct Context_t
{
    // Working directory (no / at the end)
    const char *cwd;

    // By default, 0 to 2
    fd_t stdin, stdout, stderr;
} Context;

Context *Context_new(const char *cwd);

void Context_del(Context *c);
