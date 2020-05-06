#include "io.h"

#include "types.h"
#include <string.h>

char *absPathFrom(const char *cwd, const char *p)
{
    // This is an absolute path, duplicate it
    if (p[0] == '/')
        return strdup(p);

    // Root case
    if (strcmp(cwd, "/") == 0)
    {
        char *path = malloc(2 + strlen(p));
        path[0] = '/';
        strcpy(&path[1], p);

        return path;
    }

    // Length of cwd/p\0
    size_t cwdLen = strlen(cwd);
    char *path = malloc(cwdLen + 1 + strlen(p) + 1);

    // Combine paths
    memcpy(path, cwd, cwdLen);
    path[cwdLen] = '/';
    strcpy(path + cwdLen + 1, p);

    return path;
}

char *dirPath(const char *p)
{
    size_t len = strlen(p);
    char *dir = malloc(len);

    memcpy(dir, p, len + 1);

    // Find the / location
    size_t i = len - 1;
    for (;;--i)
        if (dir[i] == '/' || i == 0)
            break;

    dir[i] = '\0';

    return dir;
}

void cutPath(const char *p, char **outDir, char **outName)
{
    size_t len = strlen(p);

    // Find the / location
    size_t i = len - 1;
    for (;;--i)
        if (p[i] == '/' || i == 0)
            break;

    *outDir = malloc(i + 1);

    size_t nameLength = len - i + 1;
    *outName = malloc(nameLength);

    memcpy(*outDir, p, i);
    (*outDir)[i] = '\0';

    // Copy also 0
    memcpy(*outName, &p[i] + 1, nameLength);
}

char *inplaceCutPath(char *s)
{
    size_t lastSlash = 0;
    for (size_t i = 0; s[i] != 0; ++i)
        if (s[i] == '/')
            lastSlash = i;
    
    // Cut here
    s[lastSlash] = '\0';

    return &s[lastSlash + 1];
}

