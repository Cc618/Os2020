#include "drivers/screen.h"
#include "drivers/console.h"
#include "syscalls/syscalls.h"
#include "io/stdstream.h"
#include "constants/fd.h"

#include <stdio.h>
// extern void stdoutPush(Stream *stream, uint8_t *data, size_t count);
// Entry from stage2

int x = 3;

void main()
{
    fillScreen('\0', 0x07);

    // setChar(10, 10, 'A', 0x07);
    // setCaret(10, 10);

    // sys_putc('!', FD_STDOUT);

    // stdoutStream.push(&stdoutStream, "TEST", 1);

    // const char *data = "TEST";
    // char a = 'A';
    // stdoutPush(NULL, &a, 1);
    

    // setChar(0, 1, '!', 0x0F);
    // setChar(1, 0, 'B', 0x0F);

    // consolePut('B');
    // consolePut('3');
    // consolePut('B');
    // consolePut('B');

    setChar(x, 0, 'A', 0x0F);
    // setChar(x++, 0, 'B', 0x0F);
    // setChar(x++, 0, 'C', 0x0F);
    // setChar(x++, 0, 'D', 0x0F);

    while (1);
}
