#include <stdio.h>
#include <string.h>

#define CAT_STDIN_BUFFER_SIZE 512

int cat(int argc, char **argv)
{
    char buf[CAT_STDIN_BUFFER_SIZE];

    gets(buf);

    puts(buf);

    return 0;

    // TODO : rm q exit
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
