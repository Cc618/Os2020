#include <stdio.h>

int help(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
    // TODO : When processes : List also apps in fs

    puts("\t\tOs2020 - v0.2");
    puts("\tTo have more help on a specific command, type <command> --help");
    puts("\tCommands :");
    puts("- cat\tConcatenate files");
    puts("- cd\tChange directory (only for shell)");
    puts("- color\tSets the color of the background / foreground of the console");
    puts("- echo\tDisplays arguments on stdout");
    puts("- exit\tTerminate shell (only for shell)");
    puts("- help\tDisplay all commands");
    puts("- ls\tList directory");
    puts("- mkdir\tCreate an empty directory");

    return 0;
}
