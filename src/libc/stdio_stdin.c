#include "stdio.h"

#include "_libc.h"
#include <stddef.h>

// Size of stdinBuffer
#define STDIN_BUFFER_SIZE 512

// The buffer which gathers all
// chars before an input
// This buffer is used like a static queue
static char stdinBuffer[STDIN_BUFFER_SIZE];

// Bounds of stdinBuffer
size_t stdinBufferStart = 0;
size_t stdinBufferEnd = 0;

// When the user presses a key, the key
// is sent here after being displayed
// The size is always one
void __libc_stdinCallback(__attribute__((unused)) void *stdinStream, uint8_t *buf, __attribute__((unused)) size_t n)
{
    __libc_stdinPut((char)*buf);
}


// Adds a new char to stdin's buffer
void __libc_stdinPut(char c)
{
    // Add
    stdinBuffer[stdinBufferEnd] = c;
    ++stdinBufferEnd;

    // Prevent overflows
    if (stdinBufferEnd >= STDIN_BUFFER_SIZE)
        stdinBufferEnd = 0;
}

// Pops the first char of stdin's buffer
// !!! Unsafe, doesn't guarantee that there
// !!! is char to get from stdin
char __libc_stdinGet()
{
    char c = stdinBuffer[stdinBufferStart];
    ++stdinBufferStart;

    // Prevent overflows
    if (stdinBufferStart >= STDIN_BUFFER_SIZE)
        stdinBufferStart = 0;

    return c;
}
