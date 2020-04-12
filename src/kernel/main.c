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



// TODO : rm
void printEntries(FSEntry **entries)
{
    for (size_t i = 0; entries[i]; ++i)
        printf("%s ", entries[i]->name);
    puts("");
}

FSEntry *findEntry(FSEntry **entries, const char *name)
{
    for (size_t i = 0; entries[i]; ++i)
        if (strcmp(name, entries[i]->name) == 0)
            return entries[i];

    return NULL;
}

void freeEntries(FSEntry **entries)
{
    for (size_t i = 0; entries[i]; ++i)
        free(entries[i]);
}






// Entry from stage2
void main()
{
    initInterrupts();

    __libc_init();

    // File system init
    fatInit();
    fsInit();


    puts("* ls root :");
    FSEntry **entries = root->ops->list(root);

    printEntries(entries);
    FSEntry dir = *findEntry(entries, "dir");

    freeEntries(entries);

    puts("* ls dir :");
    FSEntry **dirEntries = dir.ops->list(&dir);
    printEntries(dirEntries);
    freeEntries(dirEntries);

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
