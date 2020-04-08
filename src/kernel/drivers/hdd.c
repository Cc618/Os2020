#include "hdd.h"

// This driver is a wider version of stage2's ATA driver

#include "drivers/ports.h"
#include "syscalls/syscalls.h"

#include <stdint.h>

// Some ports
#define HDD_DATA            0x1F0
#define HDD_ERROR           (HDD_DATA + 1)
#define HDD_DEVICE          (HDD_DATA + 6)
#define HDD_SECTOR_COUNT    (HDD_DATA + 2)
#define HDD_LBA_LOW         (HDD_DATA + 3)
#define HDD_LBA_MID         (HDD_DATA + 4)
#define HDD_LBA_HI          (HDD_DATA + 5)
#define HDD_CMD             (HDD_DATA + 7)
#define HDD_CTRL            (HDD_DATA + 0x206)

void hddRead(size_t src, void *dst, size_t sectorCount)
{
    outb(0xE0 | ((src & 0x0F000000) >> 24), HDD_DEVICE);
    outb(0, HDD_ERROR);
    outb(sectorCount, HDD_SECTOR_COUNT);
    outb(src & 0xFF, HDD_LBA_LOW);
    outb((src & 0xFF00) >> 8, HDD_LBA_MID);
    outb((src & 0xFF0000) >> 16, HDD_LBA_HI);
    outb(0x20, HDD_CMD);

    for (size_t sector = 0; sector < sectorCount; ++sector)
    {
        // Wait until the drive is ready
        uint8_t status = inb(HDD_CMD);
        while ((status & 0x80) == 0x80 && (status & 0x01) == 0)
            status = inb(HDD_CMD);

        // Check error
        if (status & 0x01)
            // TODO : msg = Disk can't be read
            sys_fatal();

        // Read data
        for(int i = 0; i < HDD_SECTOR_SIZE / 2; ++i)
        {
            // Receive word
            uint16_t word = inw(HDD_DATA);

            // Send to destination
            ((uint16_t*)dst)[sector * (HDD_SECTOR_SIZE / 2) + i] = word;
        }
    }
}
