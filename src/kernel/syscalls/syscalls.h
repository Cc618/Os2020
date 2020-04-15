#pragma once

// All syscalls implemented in ./syscalls_handlers.c

#include <stdint.h>
#include <stddef.h>

// TODO : Update args
// --- Args --- //
// To pass args between syscalls
extern uint32_t syscallId;
extern uint32_t syscallRet;
extern uint32_t syscallArg1;
extern uint32_t syscallArg2;
extern uint32_t syscallArg3;
extern uint32_t syscallArg4;

// TODO : Update all
// --- Interrupts --- //
// When we do int 0x80 or syscall this interrupt handler is called
void onSyscall();

// --- System --- //
// Fatal error
void sys_fatal(const char *msg);

// --- IO --- //
// Appends a char to the file
// - arg1 = c : char
// - arg2 = fd : File descriptor
void sys_putc();

// Connects the stream to the callback cb
// - arg1 = fd : File descriptor 
// - arg2 = cb : Callback, functor (*)(FILE *f, uint8_t *data, size_t count)
void sys_strcon();


