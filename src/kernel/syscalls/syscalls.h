#pragma once

// All syscalls implemented in ./syscalls_*.c

#include <stdint.h>
#include <stddef.h>

#include "args.h"

// --- Interrupts --- //
// When we do int 0x80 or syscall this interrupt handler is called
void onSyscall();

// --- System --- //
// Power off
void sys_exit();

// --- IO --- //
// Appends a char to the file
// - arg1 = c : char
// - arg2 = fd : File descriptor
void sys_putc();
