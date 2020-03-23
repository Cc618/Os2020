#include "drivers/screen.h"
#include "syscalls/syscalls.h"
#include "constants/fd.h"

#include <stdio.h>

// Entry from stage2
void main()
{
    putchar('A');

    // TODO : Fill

    // sys_putc('!', FD_STDOUT);

    while (1);
}
