#pragma once

// All syscalls implemented in ./syscalls_*.c

// --- Constants --- //
// Syscalls ids
#define SYS_EXIT 0x01
#define SYS_PUTC 0x10

// --- System --- //
// Power off
void sys_exit();

// --- IO --- //
// Appends a char to the file
// - c : char
// - fd : File descriptor
void sys_putc(char c, int fd);
