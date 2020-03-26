#include "drivers/screen.h"
#include "drivers/console.h"
#include "syscalls/syscalls.h"
#include "io/stdstream.h"
#include "int/interrupts.h"

#include "syscall.h"
#include <stdio.h>

// Entry from stage2
void main()
{
    initInterrupts();

    fillScreen('\0', 0x07);

    puts("Hello");
    puts("World");
    puts("Os2020");

    // SYSC0(SYS_PUTC);
    // SYSC0(SYS_EXIT);
    // SYSC0(SYS_PUTC);

    while (1);
}
