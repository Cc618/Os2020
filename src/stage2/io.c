#include "io.h"
#include "ports.h"

// Screen constants
#define TTY_WIDTH 80
#define TTY_HEIGHT 25
#define VIDEO_MEMORY 0xB8000
#define VIDEO_MEMORY_END (VIDEO_MEMORY + TTY_WIDTH * TTY_HEIGHT * 2)

// Default display format
#define FMT_DEFAULT 0x07
#define FMT_BLUE_SCREEN 0x9F

// Ports for VGA
#define VGA_SELECT 0x3D4
#define VGA_DATA 0x3D5

// Ports for ATA
#define ATA_DATA 0x1F0
#define ATA_ERROR (ATA_DATA + 1)
#define ATA_DEVICE (ATA_DATA + 6)
#define ATA_SECTOR_COUNT (ATA_DATA + 2)
#define ATA_LBA_LOW (ATA_DATA + 3)
#define ATA_LBA_MID (ATA_DATA + 4)
#define ATA_LBA_HI (ATA_DATA + 5)
#define ATA_CMD (ATA_DATA + 7)
#define ATA_CTRL (ATA_DATA + 0x206)

unsigned int cursorPos = 0;

void fill(int error)
{
    for (int i = VIDEO_MEMORY; i < VIDEO_MEMORY_END; i += 2)
        *((short*)i) = (short)(error ? FMT_BLUE_SCREEN : FMT_DEFAULT) << 8;
}

void puts(const char *s)
{
    while (*s != '\0')
        putchar(*s++);

    newLine();
}

void fatal(const char *s)
{
    fill(1);

    cursorPos = 0;

    puts(s);

    halt();
}

void newLine()
{
    // Go to next line
    cursorPos += TTY_WIDTH;

    // Align
    cursorPos -= cursorPos % TTY_WIDTH;

    // TODO : puts end of screen

    updateCursor();
}

void putchar(char c)
{
    if (c == '\n')
        newLine();
    else
    {
        ((char*)VIDEO_MEMORY)[2 * cursorPos++] = c;

        updateCursor();
    }
}

void updateCursor()
{
    // High
    outb(0x0E, VGA_SELECT);
    outb((cursorPos & 0xFF00) >> 8, VGA_DATA);

    // Low
    outb(0x0F, VGA_SELECT);
    outb(cursorPos & 0xFF, VGA_DATA);
}

void halt()
{
    while (1);
}

void identifyDisk()
{
    // Select master drive
    outb(0xA0, ATA_DEVICE);

    // Identify command
    outb(0, ATA_SECTOR_COUNT);
    outb(0, ATA_LBA_LOW);
    outb(0, ATA_LBA_MID);
    outb(0, ATA_LBA_HI);
    outb(0xEC, ATA_CMD);

    // Test exists
    if (inb(ATA_CMD) == 0)
        fatal("Drive doesn't exist");

    // Test SATA drive
    unsigned char status;
    if (((status = inb(ATA_CMD)) & 0x80) != 0)
        if (inb(ATA_LBA_MID) != 0 || inb(ATA_LBA_HI) != 0)
            fatal("Not a SATA drive");

    // Poll
    while ((status = inb(ATA_CMD) & 8) == 0 && (status & 1) == 0);

    // Receive information
    // short data[256];
    for(int i = 0; i < 256; i++)
        /* data[i] = */ inw(ATA_DATA);

    // int handle48 = data[83];
    // Addressable sectors
    // int maxSectors = data[60] | (data[61] << 8);
}

void readDisk(int sector, void *dst)
{
    // Read 1 sector, number "sector" command
    outb(0xE0 | ((sector & 0x0F000000) >> 24), ATA_DEVICE);
    outb(0, ATA_ERROR);
    outb(1, ATA_SECTOR_COUNT);
    outb(sector & 0xFF, ATA_LBA_LOW);
    outb((sector & 0xFF00) >> 8, ATA_LBA_MID);
    outb((sector & 0xFF0000) >> 16, ATA_LBA_HI);
    outb(0x20, ATA_CMD);

    unsigned char status = inb(ATA_CMD);
    while ((status & 0x80) == 0x80 && (status & 0x01) == 0)
        status = inb(ATA_CMD);

    if (status & 0x01)
        fatal("Disk can't be read");

    // Read data
    for (int i = 0; i < 256; ++i)
    {
        // Receive word
        short word = inw(ATA_DATA);

        // Send to destination
        ((short*)dst)[i] = word;
    }
}



