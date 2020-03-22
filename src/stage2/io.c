#include "io.h"

// Screen constants
#define TTY_WIDTH 80
#define TTY_HEIGHT 25
#define VIDEO_MEMORY 0xB8000
#define VIDEO_MEMORY_END (VIDEO_MEMORY + TTY_WIDTH * TTY_HEIGHT * 2)

// Default display format
#define FMT_DEFAULT 0x07
#define FMT_BLUE_SCREEN 0x9F

// Ports
#define VGA_SELECT 0x3D4
#define VGA_DATA 0x3D5

// TODO : ren
#define DATA 0x1F0
#define ERROR (DATA + 1)
#define DEVICE (DATA + 6)
#define SECTOR_COUNT (DATA + 2)
#define LBA_LOW (DATA + 3)
#define LBA_MID (DATA + 4)
#define LBA_HI (DATA + 5)
#define CMD (DATA + 7)
#define CTRL (DATA + 0x206)

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
    // TODO :
    outb(0xA0, DEVICE);
    outb(0x0, CTRL);

    outb(0xA0, DEVICE);
    unsigned char status = inb(CMD);
    if (status == 0xFF)
        fatal("DISK"); // TODO :

    // Identify command
    outb(0xA0, DEVICE);
    outb(0, SECTOR_COUNT);
    outb(0, LBA_LOW);
    outb(0, LBA_MID);
    outb(0, LBA_HI);
    outb(0xEC, CMD);

    // Test exists
    if (inb(CMD) == 0)
        fatal("Drive doesn't exist");

    // Test SATA drive
    if (((status = inb(CMD)) & 0x80) != 0)
        if (inb(LBA_MID) != 0 || inb(LBA_HI) != 0)
            fatal("Not a SATA drive");

    // Poll
    while ((status = inb(CMD) & 8) == 0 && (status & 1) == 0);

    // Receive information
    short data[256];
    for(int i = 0; i < 256; i++)
        data[i] = inw(DATA);

    int handle48 = data[83];
    // Addressable sectors
    int maxSectors = data[60] | (data[61] << 8);
}

void readDisk(int sector, void *dst)
{
    int count = 20;

    // Read 1 sector number "sector" command
    outb(0xE0 | ((sector & 0x0F000000) >> 24), DEVICE);
    outb(0, ERROR);
    outb(1, SECTOR_COUNT);
    outb(sector & 0xFF, LBA_LOW);
    outb((sector & 0xFF00) >> 8, LBA_MID);
    outb((sector & 0xFF0000) >> 16, LBA_HI);
    outb(0x20, CMD);

    char status = inb(CMD);
    // unsigned char status = inb(CMD);
    while((status & 0x80) == 0x80 && (status & 0x01) == 0)
        status = inb(CMD);

    if (status & 0x01)
        fatal("Disk can't be read");

    // Read data
    for(int i = 0; i < 256; ++i)
    {
        // Receive word
        short word = inw(DATA);

        // Send to destination
        ((short*)dst)[i] = word;
    }
}



