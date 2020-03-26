#include "syscalls.h"

#include <stdio.h>

#include "io/stdstream.h"

void sys_putc()
{
    uint8_t c = syscallArg1;
    int fd = syscallArg2;


    // TODO : Use table
    switch (fd)
    {
    case stdout:
        // TODO : Change for stderr
    case stderr:
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
