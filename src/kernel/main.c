#include "drivers/screen.h"
#include "syscalls/syscalls.h"
#include "constants/fd.h"

#include <stdio.h>

// Entry from stage2
void main()
{
    fillScreen('\0', 0x07);

    setChar(10, 10, 'A', 0x07);
    setCaret(10, 10);

    sys_putc('!', FD_STDOUT);

    while (1);
}
