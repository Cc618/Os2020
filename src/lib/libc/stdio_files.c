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
    if (f->_fileno == INVALID_FD)
        return NULL;

    return f;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *f)
{
    if (size == 0)
        return 0;

    size_t count = size * nmemb;

    return read(f->_fileno, ptr, count) / size;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *f)
{
    if (size == 0)
        return 0;

    size_t count = size * nmemb;

    return write(f->_fileno, (void*)ptr, count) / size;
}
