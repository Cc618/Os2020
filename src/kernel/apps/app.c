#include "app.h"

#include <stdio.h>

// TODO : Find files
int exec(const char *app, __attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
    // Not found
    printf("App <%s> is not recognized\n", app);
    
    return -2;
}

int execApp(int (*entry)(int argc, char **argv), int argc, char **argv)
{
    // TODO : App stack

    return entry(argc, argv);
}

void terminateApp()
{
    // TODO :
}
