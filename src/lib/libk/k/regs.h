#pragma once

// To gather registers

#include <k/types.h>

typedef struct StackRegs_t
{
    u32 esp;
    u32 ebp;
} StackRegs;

// Used in a define to avoid calling a function
#define STACK_REGS_GET(REGS) \
    __asm__ volatile("movl %%esp, %0" : "=g" ((REGS)->esp)); \
    __asm__ volatile("movl %%ebp, %0" : "=g" ((REGS)->ebp));

#define STACK_REGS_SET(REGS) \
    __asm__ volatile("movl %0, %%esp" : "=g" ((REGS)->esp)); \
    __asm__ volatile("movl %0, %%ebp" : "=g" ((REGS)->ebp));
