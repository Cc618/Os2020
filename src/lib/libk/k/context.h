#pragma once

// A context is a struct describing a running app

typedef struct Context_t
{
    // Working directory
    const char *cwd;
} Context;

Context *Context_new(const char *cwd);

void Context_del(Context *c);
