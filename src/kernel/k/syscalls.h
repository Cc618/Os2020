#pragma once

// All syscalls

#include "types.h"

// #define DECL_SYC0(ret, id) ret id() \
//     { \

//         return r; \
//     }

// u32 id(u32 a1, u32 a2)
// {
//     // Pass args in registers


//     return r;
// }


// #define DEFN_SYSCALL1(id, num, P1)

// u32 syscall_id(P1 p1)
// {
//     u32 __res;

//     __asm__ __volatile__(
//         "push %%ebx; movl %2, %%ebx; int $0x7F; pop %%ebx"
//         : "=a" (__res)
//         : "0" (num), "r" ((int)(p1)));

//     return __res;
// }

// u32 fatal(const char *msg)
// {
//     u32 ret;

//     __asm__ __volatile__(
//         "push %%ebx;"
//         "movl %[id], %%eax;"
//         "movl %[a1], %%ebx;"
//         "int $0x80;"
//         "pop %%ebx;"
//         : "=a" (ret)
//         : [id] "r" (42), [a1] "r" ((u32) msg));

//     return ret;
// }







// --- System --- //
// Fatal error
void fatal(const char *msg);

// --- IO --- //
// Appends a char to the file
// - arg1 = c : char
// - arg2 = fd : File descriptor
void putc(char c, int fd);

// TMP
#include "io/stdstream.h"

// Connects the stream to the callback cb
// - arg1 = fd : File descriptor 
// - arg2 = cb : Callback, functor (*)(FILE *f, uint8_t *data, size_t count)
void strcon(int fd, void (*cb)(Stream *s, uint8_t *data, size_t count));









