#include "syscalls.h"

#include "drivers/screen.h"

#define FMT_FATAL ((FMT_LIGHT_BLUE << 4) | FMT_WHITE)

void sys_fatal()
{
    fillScreen('\0', FMT_FATAL);

    const char *errorMsg = "Fatal error";

    for (int x = 0; *errorMsg != 0; )
    {
        setChar(x, 0, *errorMsg, FMT_FATAL);

        ++x;
        ++errorMsg;
    }

    while (1);
}
