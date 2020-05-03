#include <k/syscalls.h>

int lsMain(int argc, char **argv)
{
    // TODO : Parse args

    // ls
    size_t n;
    char **children = ls(argv[1], &n);

    for (size_t i = 0; i < n; i++)
    {
        printf("%s ", children[i]);
        free(children[i]);
    }

    free(children);

    puts("");

    return 0;
}
