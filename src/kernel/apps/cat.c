#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <k/syscalls.h>

#define CAT_STDIN_BUFFER_SIZE 512

// Prints the content of the file
// * Returns whether there is no error
static bool printFile(const char *path)
{
    FInfo *info = finfo(path);
    FILE *f = fopen(path, "r");

    if (!f || !info)
    {
        if (finfo)
            free(finfo);

        fprintf(stderr, "File '%s' can't be opened\n", path);
        return false;
    }

    char *buf = malloc(info->size);

    fread(buf, 1, info->size, f);
    fwrite(buf, 1, info->size, stdout);

    free(buf);
    free(info);
    fclose(f);

    return true;
}

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
    }
    else
    {
        size_t i = 1;
        for ( ; i < argc && printFile(argv[i]); ++i);

        return i == argc ? 0 : -1;
    }

    return 0;
}
