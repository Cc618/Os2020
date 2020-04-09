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



// TODO : Move in fat32


// TODO :
extern int FS_SECTOR;

static char hddName[12];
static uint32_t hddRootDir;
static uint16_t hddFSInfo;

// Sector where the clusters are located
static uint16_t hddDataBegin;

// Current sector we read / write
static uint8_t sector[HDD_SECTOR_SIZE];



// TODO : first_data_sector

// A directory entry
typedef struct directory_t 
{
    char name[11];
    uint8_t flags;
    
    uint8_t reserved;

    uint8_t creationTenthSeconds;
    uint16_t creationTime;
    uint16_t creationDate;
    
    uint16_t lastAccessedDate;
    
    uint16_t firstClusterHigh;
    
    uint16_t modifiedTime;
    uint16_t modifiedDate;
    
    uint16_t firstClusterLow;
    
    uint32_t fileSize;
} __attribute__((packed)) Directory;


static Directory root;




void initHdd()
{
    // Extended Boot Record (EBPB) //
    // Read the first fs sector
    hddRead(FS_SECTOR, sector, 1);

    // Root directory
    memcpy(&hddRootDir, sector + 0x2C, 4);

    // Fs Info
    memcpy(&hddFSInfo, sector + 0x30, 2);
    
    // Name
    memcpy(hddName, sector + 0x47, 11);
    hddName[11] = '\0';


    printf("FS Info            0x%X\n", hddFSInfo);
    printf("Root dir           0x%X\n", hddRootDir);
    printf("Name               %s\n", hddName);


    // TODO : First dir
    // Load the root directory
    hddRead(FS_SECTOR + hddDataBegin + hddRootDir, sector, 1);

    memcpy(&root, sector, sizeof(Directory));

    // char rootName[12];
    // memcpy(rootName, root.name, 11);
    // rootName[11] = '\0';

    printf("Root name          %c\n", root.name[0]);
}

// TODO : strncopy

// Entry from stage2
void main()
{
    initInterrupts();

    __libc_init();

    initHdd();

    // TODO :
    while (1);

    // Launch the shell
    execApp(shellMain, 0, NULL);

    consoleNewLine();
    puts("No process running");

    while (1);
}
