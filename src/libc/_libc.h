#pragma once

// Internal functions

#include <stdint.h>
#include <stddef.h>

// Used to init the library
// Call this on app entry
void __libc_init();

// When data comes to stdin this function is called
void __libc_stdinCallback(void *stdinStream, uint8_t *buf, size_t n);
