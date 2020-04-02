#include "drivers/screen.h"
#include "drivers/console.h"
#include "syscalls/syscalls.h"
#include "io/stdstream.h"
#include "int/interrupts.h"

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

    // TODO : Init shell
    // Init display
    fillScreen('\0', (FMT_BLACK << 4) | FMT_GRAY);
    setCaret(0, 0);


    // --- Example --- //
    puts("Welcome to Os2020, type a message :");
    printf("> ");

    char msg[128];
    gets(msg);

    printf("Your message of length %d was : <%s> !\n", strlen(msg), msg);


    const char *delim = " ,'.;";

    char *p = strtok(msg, delim);
    puts(p);
    while (p = strtok(NULL, delim))
        puts(p);


    // puts(p);

    // p = strtok(NULL, " ");
    // puts(p);

    // p = strtok(NULL, " ");
    // puts(p);

    // p = strtok(NULL, " ");
    // printf("0x%p\n", p);





    while (1);
}
