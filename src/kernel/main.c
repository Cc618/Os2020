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
    puts("Hello");
    puts("World");
    printf("Os%d\n", 2020);

    stderrStream.push(NULL, "I am an error message\n", 17);

    while (1);
}
