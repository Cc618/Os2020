#include "_libc.h"

#include <stdio.h>
#include "syscall.h"

void __libc_init()
{
    // TODO : when pipes, update
    // Bind stdin to libc stdin
    strcon(stdin, __libc_stdinCallback);
}
