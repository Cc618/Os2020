#include <k/syscalls.h>
#include <string.h>

int mkdir(int argc, char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
    {
        puts("Usage : mkdir <dir1> [... <dirN>]");
        return 0;
    }

    for (size_t i = 1; i < argc; ++i)
        touch(absPath(argv[i]), true);

    return 0;
}
