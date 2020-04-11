#include "app.h"

#include "syscalls/syscalls.h"
#include <stdio.h>

// WIP Section //
// // How many apps we can execute recursively
// #define APPS_STACK_SIZE 32

// static void *appReturns[APPS_STACK_SIZE];
// static size_t appReturnI = 0;

// TODO : Find files
int exec(const char *app, __attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
    // Not found
    printf("App <%s> is not recognized\n", app);
    
    return -2;
}

int execApp(int (*entry)(int argc, char **argv), int argc, char **argv)
{
    return entry(argc, argv);
}

// WIP Section //
// int _execApp(void *from, int (*entry)(int argc, char **argv), int argc, char **argv)
// {
//     // Update app stack
//     appReturns[appReturnI] = from;

//     ++appReturnI;
//     if (appReturnI >= APPS_STACK_SIZE)
//         // TODO : msg = To many apps running
//         sys_fatal();

//     return entry(argc, argv);
// }

// void terminateApp()
// {
//     // No app running
//     if (appReturnI == 0)
//         return;
    
//     --appReturnI;

//     // TODO : Change
//     goto *appReturns[appReturnI];
// }
