#pragma once

// All syscalls implemented in ./syscalls_*.c

#include <stdint.h>

// --- Constants --- //
// To call a syscall without setting parameters to 0
#define SYSC0(ID)                           syscall(ID, 0, 0, 0, 0)
#define SYSC1(ID, arg1)                     syscall(ID, arg1, 0, 0, 0)
#define SYSC2(ID, arg1, arg2)               syscall(ID, arg1, arg2, 0, 0)
#define SYSC3(ID, arg1, arg2, arg3)         syscall(ID, arg1, arg2, arg3, 0)
#define SYSC4(ID, arg1, arg2, arg3, arg4)   syscall(ID, arg1, arg2, arg3, arg4)

// Syscalls ids
#define SYS_EXIT 0x01
#define SYS_PUTC 0x10

// --- Interrupts --- //
// When we do int 0x80 or syscall this interrupt handler is called
void onSyscall();
void syscall(uint32_t id, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);

// --- System --- //
// Power off
void sys_exit();

// --- IO --- //
// Appends a char to the file
// - c : char
// - fd : File descriptor
void sys_putc(char c, int fd);
