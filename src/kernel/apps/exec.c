#include "exec.h"

#include <string.h>
#include <stdio.h>

// TODO : When fs, find files
void exec(const char *app, int argc, char **argv)
{
    // Builtins
    if (strcmp(app, "echo") == 0)
        echo(argc, argv);
    else
    {
        printf("App <%s> is not recognized\n", app);
    }
}
