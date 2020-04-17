#pragma once

// Internal functions

#include <stdint.h>
#include <stddef.h>

// --- Init --- //
// Used to init the library
// Call this on app entry
void __libc_init();

// --- stdin --- //
// When data comes to stdin this function is called
void __libc_stdinCallback(void *stdinStream, uint8_t *buf, size_t n);

// Adds a new char to stdin's buffer
void __libc_stdinPut(char c);

// Pops the first char of stdin's buffer
char __libc_stdinGet();

// Removes the last char of the stdin buffer
void __libc_stdinDel();

