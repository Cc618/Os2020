#include "exec.h"

#include "echo.h"
#include "cat.h"
#include <string.h>
#include <stdio.h>

// TODO : When fs, find files
int exec(const char *app, int argc, char **argv)
{
    // Builtins
    if (strcmp(app, "echo") == 0)
        return echo(argc, argv);
    
    if (strcmp(app, "cat") == 0)
        return cat(argc, argv);
 
    // Not found
    printf("App <%s> is not recognized\n", app);
    
    return -2;
}
