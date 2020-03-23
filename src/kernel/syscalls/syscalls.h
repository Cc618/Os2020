#pragma once

// All syscalls implemented in ./syscalls_*.c

// --- IO --- //
// Appends a char to the file
// - c : char
// - fd : File descriptor
void sys_putc(char c, int fd);
