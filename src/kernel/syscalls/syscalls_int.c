#include "syscalls.h"

#include <stdint.h>

// To pass args when there is a syscall
uint32_t syscallId = 0;
uint32_t syscallArg1 = 0;
uint32_t syscallArg2 = 0;
uint32_t syscallArg3 = 0;
uint32_t syscallArg4 = 0;

void onSyscall()
{
    // TODO : Use table

    if (syscallId == 42)
        consolePut('4');
    else
        consolePut('-');
}

void syscall(uint32_t id, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4)
{
    // Set args and id
    syscallId = id;
    syscallArg1 = arg1;
    syscallArg2 = arg2;
    syscallArg3 = arg3;
    syscallArg4 = arg4;

    // Syscall
    __asm__ volatile ("int $0x80");
}

