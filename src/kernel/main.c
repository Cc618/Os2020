#include "int/interrupts.h"
#include "drivers/console.h"
#include "drivers/fat32.h"
#include "fs/fs.h"
#include "apps/shell.h"
#include "apps/app.h"

#include "_libc.h"

// TODO :
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// Entry from stage2
void main()
{
    initInterrupts();

    __libc_init();

    // File system init
    fatInit();
    fsInit();

    // printEntry(root);

    FSEntry **entries = fatEnumDir(root);

    for (size_t i = 0; entries[i]; ++i)
        printEntry(entries[i]);
    puts("----");
    FSEntry *dir = entries[2];

    FSEntry **dirEntries = fatEnumDir(dir);
    for (size_t i = 0; dirEntries[i]; ++i)
        printEntry(dirEntries[i]);



    // TODO : FREE
    while (1);

    // Launch the shell
    execApp(shellMain, 0, NULL);

    consoleNewLine();
    puts("No process running");

    puts("Exiting");

    fatTerminate();
    fsTerminate();

    while (1);
}
