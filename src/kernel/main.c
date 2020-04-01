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



    puts("Welcome to Os2020, type a message :");
    printf("> ");

    char msg[128];
    gets(msg);

    printf("Your message of length %d was : <%s> !\n", strlen(msg), msg);


    // // Some messages...
    // puts("Hello");

    // char a[20];
    // gets(a);
    
    // char s[20];
    // gets(s);
    
    // puts("World");
    // printf("Os%d\n", 2020);
    
    // printf("|");
    // printf("%s", s);
    // printf("|");

    while (1);
}
