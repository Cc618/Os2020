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

// TODO :
extern int FS_SECTOR;

// Entry from stage2
void main()
{
    initInterrupts();

    __libc_init();

    uint8_t sector[HDD_SECTOR_SIZE];

    // Read the first fs sector
    hddRead(FS_SECTOR, sector, 1);

    puts(sector);



    while (1);

    // Launch the shell
    execApp(shellMain, 0, NULL);

    consoleNewLine();
    puts("No process running");

    while (1);
}
