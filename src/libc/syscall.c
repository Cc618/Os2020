#include "syscall.h"

// To pass args between syscalls
extern uint32_t syscallId;
extern uint32_t syscallArg1;
extern uint32_t syscallArg2;
extern uint32_t syscallArg3;
extern uint32_t syscallArg4;
extern uint32_t syscallRet;

uint32_t syscall(uint32_t id, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4)
{
    // Set args and id
    syscallId = id;
    syscallArg1 = arg1;
    syscallArg2 = arg2;
    syscallArg3 = arg3;
    syscallArg4 = arg4;

    // Syscall
    __asm__ volatile ("int $0x80");

    return syscallRet;
}

