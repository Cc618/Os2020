#pragma once

// All syscalls implemented in ./syscalls_handlers.c

#include <stdint.h>
#include <stddef.h>
#include "k/types.h"

// TODO : Update args
// --- Args --- //
// To pass args between syscalls
// extern uint32_t syscallId;
// extern uint32_t syscallRet;
// extern uint32_t syscallArg1;
// extern uint32_t syscallArg2;
// extern uint32_t syscallArg3;
// extern uint32_t syscallArg4;

// TODO : Update all
// --- Interrupts --- //
// When we do int 0x80 or syscall this interrupt handler is called
void onSyscall();

// --- System --- //
// Fatal error
void sys_fatal(const char *msg);

// --- IO --- //
// Appends a char to the file
void sys_putc(u8 c, int fd);

// Connects the stream to the callback cb
// - arg1 = fd : File descriptor 
// - arg2 = cb : Callback, functor (*)(FILE *f, uint8_t *data, size_t count)
// TODO : rm
#include "io/stdstream.h"
void sys_strcon(int fd, void (*cb)(Stream *f, u8 *data, size_t count));
