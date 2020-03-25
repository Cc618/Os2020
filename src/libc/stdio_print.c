#include "stdio.h"

#include <kernel.h>

int putchar(int c)
{
    // TODO : Syscall
    sys_putc(c, FD_STDOUT);

    return 0;
}

int puts(const char *s)
{
    // TODO : Use fputs with CRLF

    while (*s != '\0')
    {
        int status;
        if (status = putchar(*s))
            return status;

        ++s;
    }

    return putchar('\n');
}
