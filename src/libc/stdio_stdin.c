#include "stdio.h"

#include "_libc.h"
#include <stddef.h>
#include <string.h>

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

    // Reset char
    stdinBuffer[stdinBufferStart] = '\0';

    // Update start
    ++stdinBufferStart;
    if (stdinBufferStart >= STDIN_BUFFER_SIZE)
        stdinBufferStart = 0;

    return c;
}

int getchar()
{
    // Wait for input
    while (stdinBufferStart == stdinBufferEnd);

    return (int)__libc_stdinGet();
}

char *gets(char *s)
{
    // End of string (without crlf)
    size_t end = STDIN_BUFFER_SIZE + 1;

    // Check for lines before buffer end
    for (size_t i = stdinBufferStart; i != stdinBufferEnd; i = (i + 1) % STDIN_BUFFER_SIZE)
        if (stdinBuffer[i] == '\n')
            end = i - 1;

    // We haven't found line end in the current buffer
    // Wait for end of line
    if (end == STDIN_BUFFER_SIZE + 1)
    {
        while (stdinBuffer[(stdinBufferEnd + STDIN_BUFFER_SIZE - 1) % STDIN_BUFFER_SIZE] != '\n');

        end = stdinBufferEnd - 1;
    }

    size_t length = end - stdinBufferStart;

    // Start is after end so update
    if (stdinBufferStart > end)
        length += STDIN_BUFFER_SIZE;

    // Copy string
    size_t j = 0;
    for (size_t i = stdinBufferStart; i != end; i = (i + 1) % STDIN_BUFFER_SIZE)
        s[j++] = stdinBuffer[i];

    // Remove crlf and update buffer start
    stdinBuffer[end] = '\0';
    stdinBufferStart = end + 1;

    return s;
}