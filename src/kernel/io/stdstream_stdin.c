#include "stdstream.h"

#include <drivers/console.h>

static void stdinPush(Stream *stream, uint8_t *data, size_t count)
{
    // TODO : callbacks (syscall get)
    for (size_t i = 0; i < count; ++i)
        consolePut(data[i]);
}

Stream stdinStream = {
    .push = stdinPush
};
