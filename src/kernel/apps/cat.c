#include "cat.h"

#include <stdio.h>

#define CAT_STDIN_BUFFER_SIZE 512

int cat(int argc, __attribute__((unused)) char **argv)
{
    // TODO : When fs, test argc == 1
    if (argc == 0)
    {
        char buf[CAT_STDIN_BUFFER_SIZE];
        
        while (1)
        {
            gets(buf);
            puts(buf);
        }

        return 0;
    }
    else
    {
        puts("Cat doesn't work with files now");
        return -1;
    }
}
