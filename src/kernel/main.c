#include "int/interrupts.h"
#include "drivers/console.h"
#include "apps/shell.h"
#include "apps/app.h"

#include "_libc.h"
#include <stdio.h>

// TODO :
#include "drivers/hdd.h"

// Entry from stage2
void main()
{
    initInterrupts();

    __libc_init();

    // TODO :
    unsigned char buf[1024];
    unsigned char wData[1024];
    wData[512] = 42;
    wData[511] = 0xCC;

    hddWrite(wData, 46, 2);
    hddRead(46, buf, 2);

    printf("%X %X %X %X\n", buf[0],   buf[1],   buf[2],   buf[3]);
    printf("%X %X %X %X\n", buf[508], buf[509], buf[510], buf[511]);
    printf("%X %X %X %X\n", buf[512], buf[513], buf[514], buf[515]);

    while (1);

    // Launch the shell
    execApp(shellMain, 0, NULL);

    consoleNewLine();
    puts("No process running");

    while (1);
}
