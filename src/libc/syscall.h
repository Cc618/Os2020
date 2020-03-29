#pragma once

// Syscall implementation in libc

#include <stdint.h>

// To call a syscall without setting parameters to 0
#define SYSC0(ID)                           syscall(ID, 0, 0, 0, 0)
#define SYSC1(ID, arg1)                     syscall(ID, arg1, 0, 0, 0)
#define SYSC2(ID, arg1, arg2)               syscall(ID, arg1, arg2, 0, 0)
#define SYSC3(ID, arg1, arg2, arg3)         syscall(ID, arg1, arg2, arg3, 0)
#define SYSC4(ID, arg1, arg2, arg3, arg4)   syscall(ID, arg1, arg2, arg3, arg4)

// Syscalls ids
// See doc/syscalls.md for specs
#define SYS_FATAL       0x02
#define SYS_PUTC        0x10
// TODO : GETC ????
#define SYS_GETC        0x11
// Stream connect
#define SYS_STRCON      0x20

// To raise a syscall
uint32_t syscall(uint32_t id, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);
