#include "stdio.h"

#include <kernel.h>

int putchar(int c)
{
    sys_putc(c, FD_STDOUT);

    return 0;
}
