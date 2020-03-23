#include "drivers/screen.h"
#include "syscalls/syscalls.h"
#include "constants/fd.h"

// Entry from stage2
void main()
{
    sys_putc('!', FD_STDOUT);

    while (1);
}
