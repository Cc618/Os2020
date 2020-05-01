#include "io.h"

#include "types.h"

char *absPath(Context *c, const char *p)
{
    // This is an absolute path, duplicate it
    if (p[0] == '/')
        return strdup(p);

    // Length of cwd/p\0
    size_t cwdLen = strlen(c->cwd);
    char *path = malloc(cwdLen + 1 + strlen(p) + 1);

    // Combine paths
    memcpy(path, c->cwd, cwdLen);
    path[cwdLen] = '/';
    strcpy(path + cwdLen + 1, p);

    return path;
}
