#include "int/interrupts.h"
#include "drivers/console.h"
#include "drivers/keyboard.h"
#include "drivers/fat32.h"
#include "fs/fs.h"
#include "apps/shell.h"
#include "apps/app.h"
#include "syscalls/syscalls.h"

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


// Inits all modules
static void initKernel()
{
    initInterrupts();

    __libc_init();

    // File system init
    fatInit();
    fsInit();

    // Inputs init
    keyboardInit();
}

// After init, the user can access the kernel
static void userAct()
{
// Example : ls directories //
    puts("* ls root :");
    FSEntry **rootEntries = root->ops->list(root);
    printEntries(rootEntries);

    FSEntry dir = *findEntry(rootEntries, "dir");

    delEntries(rootEntries);

    puts("* ls dir :");
    FSEntry **dirEntries = dir.ops->list(&dir);
    printEntries(dirEntries);

    puts("* ls .. :");
    FSEntry *parent = findEntry(dirEntries, "..");
    FSEntry **parentEntries = parent->ops->list(parent);
    printEntries(parentEntries);
    delEntries(parentEntries);
    
    delEntries(dirEntries);


    // Example : cat file //
    // TODO : frees
    // puts("* cat /file :");
    FSEntry **rootEntries2 = root->ops->list(root);
    FSEntry *dir2 = findEntry(rootEntries2, "dir");
    FSEntry **dirEntries2 = dir2->ops->list(dir2);
    FSEntry *file = findEntry(dirEntries2, "second");
    
    // Read
    char buf[4096];
    size_t n = FSEntry_read(file, buf, 2000);
    buf[n] = '\0';
    printf("%s\n", buf + 500);
    printf("Read %d bytes\n", n);

    delEntries(dirEntries2);
    delEntries(rootEntries2);


    // Example : Follow path //
    FSEntry *f = getEntry("/dir/../dir/second");

    if (f == NULL)
        puts("NULL");
    else
        printf("File : %s\n", f->name);


    // TODO : rm tests
    return;





    // Launch the shell
    // TODO : sys_exec
    execApp(shellMain, 0, NULL);

    consoleNewLine();
    puts("No process running");

    puts("Exiting");

}

// Terminates all modules
// !!! This function doesn't return
void terminateKernel()
{
    keyboardTerminate();
    fatTerminate();
    fsTerminate();

    while (1);
}

// Entry from stage2
void main()
{
    initKernel();

    userAct();

    terminateKernel();
}
