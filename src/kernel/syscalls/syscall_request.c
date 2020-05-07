#include "syscalls.h"

#include "drivers/ports.h"
#include <k/syscalls.h>
#include <k/types.h>

typedef u32 (*SyscallHandler)(u32, ...);

#define HANDLER(f) ((SyscallHandler)(void*)(f))

// The table that gathers syscalls
SyscallHandler syscalls[256] = {
    // Sys //
    [SYS_FATAL]         = HANDLER(sys_fatal),
    [SYS_ENTER]         = HANDLER(sys_enter),
    [SYS_TERMINATE]     = HANDLER(sys_terminate),

    // Io //
    [SYS_OPEN]          = HANDLER(sys_open),
    [SYS_READ]          = HANDLER(sys_read),
    [SYS_WRITE]         = HANDLER(sys_write),
    [SYS_CLOSE]         = HANDLER(sys_close),
    [SYS_PIPE]          = HANDLER(sys_pipe),

    // Files //
    [SYS_LS]            = HANDLER(sys_ls),
    [SYS_CONTEXT]       = HANDLER(sys_context),
    [SYS_TOUCH]         = HANDLER(sys_touch),
    [SYS_FINFO]         = HANDLER(sys_finfo),
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

    SyscallHandler sysc = syscalls[eax];

    // Interrupt end
    outb(0xA0, 0x20);
    outb(0x20, 0x20);
    __asm__ volatile("sti");

    // Invalid syscall id
    if (sysc == NULL)
        sys_fatal("Invalid syscall id");

    return sysc(ebx, ecx, edx, edi);
}
