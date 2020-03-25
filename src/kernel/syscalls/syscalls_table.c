#include "syscalls.h"

// The table that gathers syscalls

int (*syscalls[])() = {
    [SYS_EXIT] = sys_exit,
    [SYS_PUTC] = sys_putc,
};

