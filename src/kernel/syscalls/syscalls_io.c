#include "syscalls.h"

#include <stdio.h>
#include <stdbool.h>

#include "io/stdstream.h"

void sys_putc()
{
    uint8_t c = syscallArg1;
    int fd = syscallArg2;


    // TODO : Use table
    switch (fd)
    {
    case stdout:
        // TODO : Implement
        stdoutStream.push(&stdoutStream, &c, 1);
        break;

    case stderr:
        stderrStream.push(&stderrStream, &c, 1);
        break;

    case stdin:
        stdinStream.push(&stdinStream, &c, 1);
        break;
    
    default:
        // TODO : When fs, write to file
        
        // TODO : Fatal message
        sys_fatal();
        break;
    }
}

void sys_strcon()
{
    int fd = syscallArg1;
    void (*cb)(Stream*, uint8_t*, size_t) = syscallArg2;

    // TODO : Table
    switch (fd)
    {
    case stdin:
        stdinStream.push = cb;
        break;
    
    case stdout:
        stdoutStream.push = cb;
        break;

    case stderr:
        stderrStream.push = cb;
        break;
    
    default:
        sys_fatal();
        break;
    }
}
