#include "io.h"

// Screen constants
#define TTY_WIDTH 80
#define TTY_HEIGHT 25
#define VIDEO_MEMORY 0xB8000
#define VIDEO_MEMORY_END (VIDEO_MEMORY + TTY_WIDTH * TTY_HEIGHT * 2)

// Default display format
#define FMT_DEFAULT 0x07
#define FMT_BLUE_SCREEN 0x9F

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
    // TODO :
}

void halt()
{
    while (1);
}
