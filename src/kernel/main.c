#include "int/interrupts.h"
#include "drivers/console.h"
#include "drivers/keyboard.h"
#include "drivers/fat32.h"
#include "fs/fs.h"
#include "apps/shell.h"
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

    // File system init
    fatInit();
    fsInit();
    filesInit();

    // Push system context
    appContexts = Vector_new();
    Vector_add(appContexts, Context_new(""));

    // Inputs init
    keyboardInit();

    __libc_init();
}



// TODO : rm
#include "io/pipe.h"
#include "io/file.h"
#include "io/fs_file.h"
#include "k/syscalls.h"
#include "k/vector.h"
#include "k/queue.h"
#include "k/io.h"
#include "k/finfo.h"
#include "syscalls/syscalls.h"


static void p(void *item)
{
    printf("- %d\n", *(int*)item);
}





int child(int argc, char **argv)
{
    puts("child");

    return 43;
}

int myApp(int argc, char **argv)
{
    puts("myApp()");

    Context *c = currentContext();
    printf("cwd = %s\n", c->cwd);

    FILE *f = fopen("first", "r");
    puts(f ? "File exists" : "File not found");
    fclose(f);

    return 42;
}

#include <k/buffer.h>

// After init, the user can access the kernel
static void userAct()
{
    // TODO : shell update (see / only at root)
    // TODO : cat
    // TODO : Clean code (fs.c:16, app.c) + TMP
    // TODO : v0.2 !


    // // Touch
    // touch("/dir/touched", false);
    // touch("touched2", false);
    // touch("/dir/newsubdir", true);
    // touch("alajdadjad/touched3", false);

    // // Info
    // FInfo *info = finfo("/dir/newsubdir");
    // printf("Size = %d, dir = %s\n", info->size, info->directory ? "true" : "false");
    // free(info);
    // info = finfo("/dir/second");
    // printf("Size = %d, dir = %s\n", info->size, info->directory ? "true" : "false");
    // free(info);


    // // ls
    // size_t n;
    // char **children = ls("/dir/newsubdir", &n);

    // printf("> %d\n", n);

    // for (size_t i = 0; i < n; i++)
    // {
    //     printf("- %s\n", children[i]);
    //     free(children[i]);
    // }

    // free(children);


    // // context
    // printf("Context cwd : '%s' (%p)\n", context()->cwd, context()->cwd);


    // // TMP
    // return;





    // Launch the shell
    char *shellArgv[2];
    shellArgv[0] = "";
    shellArgv[1] = "/dir";

    sys_enter(Context_new(""), shellMain, 2, shellArgv);

    consoleNewLine();
    puts("No process running");

    puts("Exiting");

}

// Terminates all modules
// !!! This function doesn't return
void terminateKernel()
{
    // Terminate system context
    Vector_del(appContexts);

    keyboardTerminate();
    filesTerminate();
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
