#include "drivers/screen.h"
#include "drivers/console.h"
#include "syscalls/syscalls.h"
#include "io/stdstream.h"
#include "int/interrupts.h"
#include "apps/shell.h"

#include "syscall.h"
#include "_libc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

// Entry from stage2
void main()
{
    initInterrupts();

    // Init libc
    __libc_init();

    // Init shell
    shellInit();

    while (1);
}
