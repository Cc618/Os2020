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



    // puts("Welcome to Os2020, type a message :");
    // printf("> ");

    // char msg[128];
    // gets(msg);

    // printf("Your message of length %d was : <%s> !\n", strlen(msg), msg);


    // TODO : printf %X disp 0 when 0 not nothing
    // TODO : printf %p disp (nil) when 0




    int *a = malloc(sizeof(int));
    *a = 0x42424242;
    int *c = malloc(sizeof(int));

    // free(a);


    int *myNb = malloc(sizeof(int));

    *myNb = 0xAABBCCDD;

    free(c);

    int *b = malloc(sizeof(int));

    puts("!");

    // printf("a           = 0x%X\n", *a);
    // printf("a next      = 0x%p\n", *(a - 2));
    // printf("&myNb       = 0x%p\n", myNb);
    // printf("myNb        = 0x%X\n", *myNb);
    // printf("Head size   = 0x%X\n", *(myNb - 4));
    // printf("Head prev   = 0x%p\n", *(myNb - 3));
    // printf("Head next   = 0x%p\n", *(myNb - 2));
    // printf("Head pad    = 0x%X\n", *(myNb - 1));

    // printf("a next   = 0x%p\n", *(a - 2));
    // printf("n prev   = 0x%p\n", *(myNb - 3));
    // printf("n        = 0x%X\n", *myNb);


    // puts("A :");
    // dbgHead(a);
    // puts("N :");
    // dbgHead(myNb);

    // free(a);

    // puts("A :");
    // // dbgHead(a);
    // puts("N :");
    // dbgHead(myNb);

    while (1);
}
