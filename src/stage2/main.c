#include "io.h"
#include "ports.h"

void main()
{
    // Clear screen
    flush();

    // Print hello world
    puts("Hello world !");

    // load(4, 0x1000000);


    // // TODO : OFFSET
    // if (*(char*)0xB8000 == 'A')
    //     puts("OK");
    // else
    //     puts("?");

    // loadAsm();

    // putchar(buffer[0]);
    
    // *(char*)0xB8001 = 0x0F;

    char dst[512];

    identifyDisk();
    readDisk(6, dst);

    puts(dst);

    puts("End");

    // TODO : jmp to kernel
    while (1);
}
