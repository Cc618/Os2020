#include "exec.h"

#include "echo.h"
#include "cat.h"
#include <string.h>
#include <stdio.h>

// TODO : Find files
int exec(const char *app, __attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
    // Not found
    printf("App <%s> is not recognized\n", app);
    
    return -2;
}
