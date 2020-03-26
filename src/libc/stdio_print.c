#include "stdio.h"

#include "syscall.h"

int putchar(int c)
{
    // TODO : Syscall
    SYSC2(SYS_PUTC, c, stdout);

    return 0;
}

int puts(const char *s)
{
    // TODO : Use fputs with CRLF

    while (*s != '\0')
    {
        int status;
        if ((status = putchar(*s)))
            return status;

        ++s;
    }

    return putchar('\n');
}
