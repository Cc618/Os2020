#include "io.h"

void main()
{
    // Clear screen
    flush();

    // Print hello world
    puts("Hello world !");
    puts("Booting...");

    fatal("ho ho");

    // TODO : jmp to kernel
    while (1);
}

