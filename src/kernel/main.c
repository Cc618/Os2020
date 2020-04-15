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


FSEntry *findEntry(FSEntry **entries, const char *name)
{
    for (size_t i = 0; entries[i]; ++i)
        if (strcmp(name, entries[i]->name) == 0)
            return entries[i];

    return NULL;
}

// TODO : rm
void printEntries(FSEntry **entries)
{
    for (size_t i = 0; entries[i]; ++i)
        printf("%s ", entries[i]->name);
    puts("");
}

void freeEntries(FSEntry **entries)
{
    for (size_t i = 0; entries[i]; ++i)
        FSEntry_del(entries[i]);
    
    free(entries);
}

// Retrieve an entry at this absolute path
// We can use \ or /
// Returns NULL if not found
FSEntry *getEntry(const char *path)
{
    // Don't parse root
    if (path[0] == '/' || path[0] == '\\')
        ++path;

    char *p = strdup(path);
    const char *delim = "/\\";
    char *part = strtok(p, delim);
    
    // Current directory we parse
    const FSEntry *current = root;

    do
    {
        // ls
        FSEntry **entries = FSEntry_list(current);

        if (current != root)
            FSEntry_del(current);

        if (entries == NULL)
            return NULL;

        current = findEntry(entries, part);

        //  Free all entries excluding current
        for (size_t i = 0; entries[i]; ++i)
            // Don't delete the first directory
            if (current == root || entries[i] != current)
                FSEntry_del(entries[i]);
        
        free(entries);

        if (current == NULL)
            return NULL;
    } while ((part = strtok(NULL, delim)));

    free(p);

    return current;
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
// // Example : ls directories //
    // puts("* ls root :");
    // FSEntry **rootEntries = root->ops->list(root);
    // printEntries(rootEntries);

    // FSEntry dir = *findEntry(rootEntries, "dir");

    // freeEntries(rootEntries);

    // puts("* ls dir :");
    // FSEntry **dirEntries = dir.ops->list(&dir);
    // printEntries(dirEntries);

    // puts("* ls .. :");
    // FSEntry *parent = findEntry(dirEntries, "..");
    // FSEntry **parentEntries = parent->ops->list(parent);
    // printEntries(parentEntries);
    // freeEntries(parentEntries);
    
    // freeEntries(dirEntries);


    // // Example : cat file //
    // // TODO : frees
    // // puts("* cat /file :");
    // FSEntry **rootEntries = root->ops->list(root);
    // FSEntry *dir = findEntry(rootEntries, "dir");
    // FSEntry **dirEntries = dir->ops->list(dir);
    // FSEntry *file = findEntry(dirEntries, "second");
    
    // // Read
    // char buf[4096];
    // size_t n = FSEntry_read(file, buf, 2000);
    // buf[n] = '\0';
    // printf("%s\n", buf + 500);
    // printf("Read %d bytes\n", n);

    // freeEntries(rootEntries);


    // // Example : Follow path //
    // FSEntry *f = getEntry("/dir/../dir/second");

    // if (f == NULL)
    //     puts("NULL");
    // else
    //     printf("File : %s\n", f->name);


    // while (1);



    sys_fatal("My message");


    // Launch the shell
    // TODO : sys_exec
    // execApp(shellMain, 0, NULL);

    consoleNewLine();
    puts("No process running");

    puts("Exiting");

}

// Terminates all modules
// !!! This function doesn't return
static void terminateKernel()
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
