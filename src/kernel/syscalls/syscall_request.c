#include "syscalls.h"

#include <k/syscalls.h>
#include <k/types.h>

// The table that gathers syscalls
u32 (*syscalls[256])() = {
    // Sys //
    [SYS_FATAL] = sys_fatal,
    [SYS_ENTER] = sys_enter,
    [SYS_TERMINATE] = sys_terminate,

    // Io //
    [SYS_STRCON] = sys_strcon,
    [SYS_READ] = sys_read,
    [SYS_WRITE] = sys_write,
    [SYS_CLOSE] = sys_close,
    [SYS_PIPE] = sys_pipe,
};

u32 onSyscall()
{
    u32 eax,
        ebx,
        ecx,
        edx,
        edi;
    
    // Retrieve args within registers
    __asm__ volatile("movl %%eax, %0" : "=g" (eax));
    __asm__ volatile("movl %%ebx, %0" : "=g" (ebx));
    __asm__ volatile("movl %%ecx, %0" : "=g" (ecx));
    __asm__ volatile("movl %%edx, %0" : "=g" (edx));
    __asm__ volatile("movl %%edi, %0" : "=g" (edi));

    u32 (*sysc)(u32, ...) = syscalls[eax];

    // Interrupt end
    outb(0xA0, 0x20);
    outb(0x20, 0x20);

    // Invalid syscall id
    if (sysc == NULL)
        sys_fatal("Invalid syscall id");

    return sysc(ebx, ecx, edx, edi);
}
