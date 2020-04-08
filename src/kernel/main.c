#include "int/interrupts.h"
#include "drivers/console.h"
#include "apps/shell.h"
#include "apps/app.h"

#include "_libc.h"
#include <stdio.h>

// Entry from stage2
void main()
{
    initInterrupts();

    __libc_init();

    // Launch the shell
    execApp(shellMain, 0, NULL);

    consoleNewLine();
    puts("No process running");

    while (1);
}
