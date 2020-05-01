#pragma once

// All syscalls (kernel) implemented in ./syscalls_handlers.c

#include <stdint.h>
#include <stddef.h>
#include <k/types.h>
#include <k/context.h>

// --- Interrupts --- //
// When we do int 0x80 or syscall this interrupt handler is called
u32 onSyscall();

// --- System --- //
// Fatal error
void sys_fatal(const char *msg);

// Executes an application by executing entry (main)
// Exec without loading and preparing the program in ram
int sys_enter(Context *c, int (*entry)(int argc, char **argv), int argc, char **argv);

// Terminates the current process
// Defined in handlers.asm
extern void sys_terminate();

// --- IO --- //
// Opens a file at the absolute path 'path'
// * Returns -1 on error
fd_t sys_open(const char *path, u8 mode);

// Reads count bytes of the file associated to fd in buffer
// * Returns how many bytes read
ssize_t sys_read(fd_t fd, void *buffer, size_t count);

// Writes count bytes of buffer in the file associated to fd
// * Returns how many bytes written
ssize_t sys_write(fd_t fd, void *buffer, size_t count);

// Closes a file
void sys_close(fd_t fd);

// Creates a pipe
// * Returns the file descriptor of the pipe
fd_t sys_pipe();
