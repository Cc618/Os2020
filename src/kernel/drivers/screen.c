#include "screen.h"

#include "ports.h"
#include <string.h>

// Video memory addresses
#define VIDEO_MEMORY 0xB8000
#define VIDEO_MEMORY_END (VIDEO_MEMORY + SCREEN_WIDTH * SCREEN_HEIGHT * 2)

// Ports for VGA
#define VGA_SELECT 0x3D4
#define VGA_DATA 0x3D5

// Like setChar but with offset instead of position
// !!! offset is supposed to be within screen bounds
void setCharOffset(unsigned int offset, char c, char fmt)
{
    char *px = (char*)(VIDEO_MEMORY + offset * 2);

    *px = c;
    *++px = fmt;
}

// Sets the char c at pos (x, y) with format fmt
int setChar(unsigned int x, unsigned int y, char c, char fmt)
{
    if (x > SCREEN_WIDTH)
        return -1;

    if (y > SCREEN_HEIGHT)
        return -1;

    setCharOffset(y * SCREEN_WIDTH + x, c, fmt);

    return 0;
}

// Fills the screen
void fillScreen(char c, char fmt)
{
    // The pixel data
    short px = c | (fmt << 8);

    // Fill each pixel
    for (short *pxPtr = (short*)VIDEO_MEMORY; pxPtr < (short*)VIDEO_MEMORY_END; ++pxPtr)
        *pxPtr = px;
}

int setCaret(unsigned int x, unsigned int y)
{
    if (x > SCREEN_WIDTH)
        return -1;

    if (y > SCREEN_HEIGHT)
        return -1;

    int cursorPos = y * SCREEN_WIDTH + x;

    // High
    outb(0x0E, VGA_SELECT);
    outb((cursorPos & 0xFF00) >> 8, VGA_DATA);

    // Low
    outb(0x0F, VGA_SELECT);
    outb(cursorPos & 0xFF, VGA_DATA);

    return 0;
}

void screenScroll(size_t pixels)
{
    // TODO : assert(pixels < SCREEN_WIDTH * SCREEN_HEIGHT && pixels != 0);

    memmove((void*)VIDEO_MEMORY, (void*)(VIDEO_MEMORY + 2 * pixels), 2 * SCREEN_WIDTH * SCREEN_HEIGHT * pixels);

    for (size_t px = SCREEN_WIDTH * SCREEN_HEIGHT; px >= pixels; ++px)
        ((short*) VIDEO_MEMORY)[px] = 0;
}
