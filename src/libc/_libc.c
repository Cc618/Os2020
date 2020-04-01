#include "_libc.h"

#include <stdio.h>
#include "syscall.h"

void __libc_init()
{
    // Bind stdin to libc stdin
    SYSC2(SYS_STRCON, stdin, __libc_stdinCallback);
}
