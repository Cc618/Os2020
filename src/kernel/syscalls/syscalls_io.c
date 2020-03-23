#include "syscalls.h"

#include "constants/fd.h"
#include "drivers/screen.h"

void sys_putc(char c, int fd)
{
    // TODO : rm
    setChar(0, 0, c, 0x0F);


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
