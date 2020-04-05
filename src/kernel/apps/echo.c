#include "echo.h"

#include <stdio.h>

int echo(int argc, char **argv)
{
    for (int i = 0; i < argc - 1; ++i)
        printf("%s ", argv[i]);

    if (argc != 0)
       printf("%s", argv[argc - 1]);
    
    putchar('\n');
}
