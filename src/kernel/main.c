#include "drivers/screen.h"
#include "drivers/console.h"
#include "syscalls/syscalls.h"
#include "io/stdstream.h"
#include "int/interrupts.h"

#include "syscall.h"
#include "_libc.h"
#include <stdio.h>
#include <stdarg.h>

// Entry from stage2
void main()
{
    initInterrupts();

    // Init libc
    __libc_init();

    // TODO : Init shell
    // Init display
    fillScreen('\0', (FMT_BLACK << 4) | FMT_GRAY);
    setCaret(0, 0);

    // Some messages...
    puts("Hello");
    puts("World");
    printf("Os%d\n", 2020);

    // syscallArg1 = stdin;
    // syscallArg2 = cb;
    // sys_strcon();
    // SYSC2(SYS_STRCON, stdin, cb);

    // stderrStream.push(NULL, "I am an error message\n", 22);

    while (1);
}
