#pragma once

// All syscalls (user)

#include "types.h"

// --- Id --- //
// Syscalls ids
// See doc/syscalls.md for specs
#define SYS_FATAL       0x02
#define SYS_PUTC        0x10
// Stream connect
#define SYS_STRCON      0x20


// --- System --- //
// Fatal error
extern void fatal(const char *msg);

// --- IO --- //
// Appends a char to the file
// - arg1 = c : char
// - arg2 = fd : File descriptor
extern void putc(char c, int fd);

// Connects the stream to the callback cb
// - arg1 = fd : File descriptor 
// - arg2 = cb : Callback, functor (*)(FILE *f, uint8_t *data, size_t count)
extern void strcon(int fd, void (*cb)(void *s, uint8_t *data, size_t count));
