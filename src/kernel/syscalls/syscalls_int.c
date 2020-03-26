#include "syscalls.h"

#include "syscall.h"
#include <stdint.h>

// To pass args when there is a syscall
uint32_t syscallId = 0;
uint32_t syscallArg1 = 0;
uint32_t syscallArg2 = 0;
uint32_t syscallArg3 = 0;
uint32_t syscallArg4 = 0;

// The table that gathers syscalls
void (*syscalls[256])() = {
    [SYS_FATAL] = sys_fatal,
    [SYS_PUTC] = sys_putc,
};

void onSyscall()
{
    void (*sysc)() = syscalls[syscallId];

    // TODO : Fatal
    if (sysc == NULL)
        return;

    sysc();
}
