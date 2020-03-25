#include "drivers/screen.h"
#include "drivers/console.h"
#include "syscalls/syscalls.h"
#include "io/stdstream.h"
#include "constants/fd.h"
#include "int/interrupts.h"
#include "syscalls/syscalls.h"

#include <stdio.h>

// Entry from stage2
void main()
{
    // TODO : Interrupts in stage2 ?
    initInterrupts();

    fillScreen('\0', 0x07);

    puts("Hello");
    puts("World");
    puts("Os2020");

    SYSC0(42);

    while (1);
}
