#include "syscalls.h"

#include "constants/fd.h"
#include "io/stdstream.h"

void sys_putc(char c, int fd)
{
    switch (fd)
    {
    case FD_STDOUT:
        // TODO : Change for stderr
    case FD_STDERR:
        // TODO : Implement

        // TODO : Buffered io
        stdoutStream.push(&stdoutStream, &c, 1);
        break;
    
    default:
        // TODO : When fs, write to file
        // TODO : fatal
        break;
    }
}
