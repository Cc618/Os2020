#include "cat.h"

#include <stdio.h>
#include <string.h>

#define CAT_STDIN_BUFFER_SIZE 512

int cat(int argc, __attribute__((unused)) char **argv)
{
    if (argc == 1)
    {
        char buf[CAT_STDIN_BUFFER_SIZE];

        while (1)
        {
            gets(buf);

            if (strcmp(buf, "q") == 0)
                break;

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
