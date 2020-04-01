#include "stdio.h"

#include "_libc.h"
#include <stddef.h>

// Size of stdinBuffer
#define STDIN_BUFFER_SIZE 512

// The buffer which gathers all
// chars before an input
static char stdinBuffer[STDIN_BUFFER_SIZE];

// End of stdinBuffer
size_t stdinBufferIndex = 0;

// This function is defined in _libc.h
// When the user presses a key, the key
// is sent here after being displayed
void __libc_stdinCallback(void *stdinStream, uint8_t *buf, size_t n)
{
}
