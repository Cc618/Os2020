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

    printEntry(root);

    FSEntry **entries = fatEnumDir(root);

    printEntry(entries[0]);
    printEntry(entries[1]);

    FSEntry *dir = entries[1];

    puts("");
    printf("%x\n", ((FatFSEntryData*)dir->data)->cluster);

    FSEntry **dirEntries = fatEnumDir(dir);
    printEntry(dirEntries[0]);
    printEntry(dirEntries[1]);
    printEntry(dirEntries[2]);


    // TODO :
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
