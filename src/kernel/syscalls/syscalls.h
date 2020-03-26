#pragma once

// All syscalls implemented in ./syscalls_*.c

#include <stdint.h>
#include <stddef.h>

// --- Args --- //
// To pass args between syscalls
extern uint32_t syscallId;
extern uint32_t syscallArg1;
extern uint32_t syscallArg2;
extern uint32_t syscallArg3;
extern uint32_t syscallArg4;

// --- Interrupts --- //
// When we do int 0x80 or syscall this interrupt handler is called
void onSyscall();

// --- System --- //
// Fatal error
void sys_fatal();

// --- IO --- //
// Appends a char to the file
// - arg1 = c : char
// - arg2 = fd : File descriptor
void sys_putc();
