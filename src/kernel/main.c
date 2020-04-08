#include "int/interrupts.h"
#include "drivers/console.h"
#include "apps/shell.h"
#include "apps/app.h"

#include "_libc.h"
#include <stdio.h>

// TODO :
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "drivers/hdd.h"

// TODO : Find with kernel end
#define FS_BEGIN 48

// Entry from stage2
void main()
{
    initInterrupts();

    __libc_init();

  
    // TODO : Test
    const char *data = "My data to save";
    uint8_t sector[HDD_SECTOR_SIZE];

    char input[20];

    puts("Write / Read ?");
    gets(input);

    // Write
    if (strcmp(input, "w") == 0 || strcmp(input, "write") == 0)
    {
        strcpy(sector, data);
        hddWrite(sector, FS_BEGIN, 1);

        puts("Written");
        puts(data);
    }
    else
    {
        // Read
        hddRead(FS_BEGIN, sector, 1);
        puts(sector);
    }

    while (1);

    // Launch the shell
    execApp(shellMain, 0, NULL);

    consoleNewLine();
    puts("No process running");

    while (1);
}
