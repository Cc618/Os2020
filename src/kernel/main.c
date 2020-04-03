#include "drivers/screen.h"
#include "drivers/console.h"
#include "syscalls/syscalls.h"
#include "io/stdstream.h"
#include "int/interrupts.h"
#include "apps/shell.h"

#include "syscall.h"
#include "_libc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

// Entry from stage2
void main()
{
    initInterrupts();

    // Init libc
    __libc_init();

    // Init shell
    shellInit();

    // // --- Example --- //
    // puts("Welcome to Os2020, type a message :");
    // printf("> ");

    // char msg[128];
    // gets(msg);

    // printf("Your message of length %d was : <%s> !\n", strlen(msg), msg);


    // // --- Test --- //
    // const char *delim = " ,'.;";

    // char *p = strtok(msg, delim);
    // puts(p);
    // while (p = strtok(NULL, delim))
    //     puts(p);

    while (1);
}
