#pragma once

// All syscalls (user)

#include "types.h"

// --- Id --- //
// !!! These definitions should be the same as syscalls.inc's definitions
// Syscalls ids
// See doc/syscalls.md for specs
#define SYS_FATAL       0x02
#define SYS_ENTER       0x03
#define SYS_TERMINATE   0x04

#define SYS_PUTC        0x10
// Stream connect
#define SYS_STRCON      0x20


// --- System --- //
// Fatal error
extern void fatal(const char *msg);

// Calls the entry of an application
extern int enter(int (*entry)(int argc, char **argv), int argc, char **argv);

// Terminates the current program
extern int terminate();

// --- IO --- //
// Appends a char to the file
// - arg1 = c : char
// - arg2 = fd : File descriptor
extern void putc(char c, int fd);

// Connects the stream to the callback cb
// - arg1 = fd : File descriptor 
// - arg2 = cb : Callback, functor (*)(FILE *f, uint8_t *data, size_t count)
extern void strcon(int fd, void (*cb)(void *s, uint8_t *data, size_t count));
