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
        entries[i]->ops->list(entries[i]);
    
    free(entries);
}






// Entry from stage2
void main()
{
    initInterrupts();

    __libc_init();

    // File system init
    fatInit();
    fsInit();


    // Example : ls directories //
    puts("* ls root :");
    FSEntry **rootEntries = root->ops->list(root);
    printEntries(rootEntries);

    FSEntry dir = *findEntry(rootEntries, "dir");

    freeEntries(rootEntries);

    puts("* ls dir :");
    FSEntry **dirEntries = dir.ops->list(&dir);
    printEntries(dirEntries);

    puts("* ls .. :");
    FSEntry *parent = findEntry(dirEntries, "..");
    FSEntry **parentEntries = parent->ops->list(parent);
    printEntries(parentEntries);
    freeEntries(parentEntries);
    
    freeEntries(dirEntries);


    // TODO : Encapsulate ops of FSEntry

    // // Example : cat file //
    // puts("* cat /file :");
    // FSEntry **rootEntries = root->ops->list(root);
    // FSEntry *file = findEntry(rootEntries, "file");
    // fatRead(file);

    // freeEntries(rootEntries);



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
