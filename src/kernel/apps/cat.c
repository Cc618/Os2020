#include <stdio.h>
#include <string.h>

#define CAT_STDIN_BUFFER_SIZE 512

int cat(int argc, char **argv)
{
    if (argc == 1)
    {
        char buf[CAT_STDIN_BUFFER_SIZE];

        while (1)
        {
            gets(buf);

            printf("%s", buf);
        }

        return 0;
    }
    else
    {
        puts("Cat doesn't work with files now");
        return -1;
    }
}
