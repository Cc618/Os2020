#include "syscalls.h"

#include "constants/fd.h"

void sys_putc(char c, int fd)
{
    // TODO : rm
    *(short*)0xB8000 = c | 0x0700;

    switch (fd)
    {
    case FD_STDOUT:
        // TODO : Change for stderr
    case FD_STDERR:
        // TODO : Implement
        break;
    
    default:
        // TODO : When fs, write to file
        // TODO : fatal
        break;
    }
}
