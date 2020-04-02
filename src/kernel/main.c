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


    // // --- Example --- //
    // puts("Welcome to Os2020, type a message :");
    // printf("> ");

    // char msg[128];
    // gets(msg);

    // printf("Your message of length %d was : <%s> !\n", strlen(msg), msg);


    // TODO : Multiple delimiters


    const char *_s = "Hello world   ! I'   m Cc";
    char *s = strdup(_s);

    puts(s);

    char *p = strtok(s, " '");
    puts(p);
    while (p = strtok(NULL, " '"))
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
