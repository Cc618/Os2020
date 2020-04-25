#include "stdio.h"

#include <k/syscalls.h>

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

// TODO : Relative path
FILE *fopen(const char *path, __attribute__((unused)) const char *mode)
{
    FILE *f = malloc(sizeof(FILE));

    f->_fileno = open(path);

    return f;
}
