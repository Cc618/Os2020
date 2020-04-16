#include "syscalls.h"

#include "syscall.h"
#include "k/types.h"

// The table that gathers syscalls
void (*syscalls[256])() = {
    [SYS_FATAL] = sys_fatal,
    [SYS_PUTC] = sys_putc,
    [SYS_STRCON] = sys_strcon,
};

void onSyscall()
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

    // Invalid syscall id
    if (sysc == NULL)
        sys_fatal("Invalid syscall id");

    sysc(ebx, ecx, edx, edi);
}
