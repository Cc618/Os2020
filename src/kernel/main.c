#include "drivers/screen.h"
#include "drivers/console.h"
#include "syscalls/syscalls.h"
#include "io/stdstream.h"
#include "constants/fd.h"

#include <stdio.h>

// Entry from stage2
void main()
{
    fillScreen('\0', 0x07);

    // for (int i = 0; i < 38; ++i)
    // {
    //     consolePut('O');
    //     consolePut('S');
    // }

    // consolePut('2');
    // consolePut('0');
    // consolePut('2');
    // consolePut('0');

    // stdoutStream.push(&stdoutStream, "HELLO", 5);

    puts("Hello");
    puts("World");
    puts("Os2020");

    while (1);
}
