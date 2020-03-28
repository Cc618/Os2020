#include "drivers/screen.h"
#include "drivers/console.h"
#include "syscalls/syscalls.h"
#include "io/stdstream.h"
#include "int/interrupts.h"

#include "syscall.h"
#include <stdio.h>
#include <stdarg.h>

// Entry from stage2
void main()
{
    initInterrupts();

    // Init display
    fillScreen('\0', (FMT_BLACK << 4) | FMT_GRAY);
    setCaret(0, 0);

    // Some messages...
    // puts("Hello");
    // puts("World");
    // puts("Os2020");

    printf("This %x is %d a %s ", 0xA0, -42, "test");

    stderrStream.push(NULL, "This is an error\n", 17);

    while (1);
}
