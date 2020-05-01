#include "stdio.h"

#include <k/syscalls.h>
#include <k/io.h>
#include <string.h>

FILE *stdin;
FILE *stdout;
FILE *stderr;

int fclose(FILE *f)
{
    if (!f)
        return -1;

    close(f->_fileno);
    free(f);

    return 0;
}

FILE *fopen(const char *path, __attribute__((unused)) const char *mode)
{
    FILE *f = malloc(sizeof(FILE));

    u8 modeFlags = 0;
    if (strcmp(mode, "r") == 0)
        modeFlags = F_READ;
    else if (strcmp(mode, "w") == 0)
        modeFlags = F_WRITE;
    else if (strcmp(mode, "r+") == 0 || strcmp(mode, "w+") == 0)
        modeFlags = F_WRITE | F_READ;
    else if (strcmp(mode, "a") == 0)
        modeFlags = F_APPEND;
    else if (strcmp(mode, "a+") == 0)
        modeFlags = F_APPEND | F_READ;

    f->_fileno = open(path, modeFlags);

    // Error
    if (f->_fileno == -1)
        return NULL;

    return f;
}
