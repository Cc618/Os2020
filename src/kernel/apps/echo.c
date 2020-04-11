#include "echo.h"

#include <stdio.h>

int echo(int argc, char **argv)
{
    for (int i = 1; i < argc - 1; ++i)
        printf("%s ", argv[i]);

    if (argc > 1)
       printf("%s", argv[argc - 1]);
    
    putchar('\n');

    return 0;
}
