#include "constants.h"
#include "io.h"
#include "ports.h"

// TODO : auto
#define STAGE2_SECTOR 5

void loadKernel()
{
    int loadedSectors = 0;
    int *loadDestination = (int*)KERNEL_OFFSET;

    do
    {
        // Load a sector
        readDisk(loadedSectors + STAGE2_SECTOR, loadDestination);

        ++loadedSectors;

        if (loadedSectors > KERNEL_MAX_SIZE)
            fatal("Damaged kernel, failed to find end of kernel");

        // Set the location for next sector
        loadDestination += 512 / sizeof(int);

    // Load until the magic number
    } while (loadDestination[-1] != END_OF_KERNEL);
}

void main()
{
    // Clear screen
    flush();

    // Print hello world
    puts("Hello world !");

    // Check for disk IO errors
    identifyDisk();

    loadKernel();
    
    puts("Kernel loaded");

    // TODO : jmp to kernel
    while (1);
}
