#include "stdio.h"

#include "_libc.h"
#include <k/syscalls.h>
#include <k/types.h>
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
    // Backspace
    if (*buf == 0x08)
        __libc_stdinDel();
    else
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

    // Update start
    ++stdinBufferStart;
    if (stdinBufferStart >= STDIN_BUFFER_SIZE)
        stdinBufferStart = 0;

    return c;
}

void __libc_stdinDel()
{
    // Swallow char
    if (stdinBufferStart != stdinBufferEnd)
        stdinBufferEnd = (stdinBufferEnd + STDIN_BUFFER_SIZE - 1) % STDIN_BUFFER_SIZE;
}

int getchar()
{
    // Wait for input
    while (stdinBufferStart == stdinBufferEnd);

    return (int)__libc_stdinGet();
}

char *gets(char *s)
{
    char *oldS = s;
    for (;;)
    {
        int c = fgetc(stdin);

        if (c == EOF)
        {
            *s = '\0';
            return s == oldS ? NULL : oldS;
        }

        *s = (char)c;

        ++s;

        if (c == '\n')
        {
            *s = '\0';

            return oldS;
        }
    }
}

int fgetc(FILE *f)
{
    char c;

    ssize_t result;
    do
    {
        result = read(f->_fileno, &c, 1);

        if (result == -1)
            return EOF;
    } while (result != 1);

    return (int)c;
}

char *fgets(char *s, int n, FILE *f)
{
    // TODO : Check EOF
    if (n == 0)
        return s;

    char *oldS = s;
    size_t totalRead = 0;
    do
    {
        ssize_t result = read(f->_fileno, s, n - 1 - totalRead);

        // Error encountered
        if (result < 0)
            goto ret;

        // Test end
        for (size_t i = 0; i < result; ++i)
        {
            if (s[i] == '\n')
            {
                totalRead += i + 1;
                goto ret;
            }
        }

        totalRead += result;
        s += result;
    }
    while (totalRead < n - 1);

ret:;
    oldS[totalRead] = '\0';

    return oldS;
}







