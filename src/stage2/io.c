#include "io.h"

// Screen constants
#define TTY_WIDTH 80
#define TTY_HEIGHT 25
#define VIDEO_MEMORY 0xB8000
#define VIDEO_MEMORY_END (VIDEO_MEMORY + TTY_WIDTH * TTY_HEIGHT * 2)

// Default display format
#define FMT_DEFAULT 0x07

unsigned int cursorPos = 0;

void flush()
{
    for (int i = VIDEO_MEMORY; i < VIDEO_MEMORY_END; i += 2)
        *((short*)i) = (short)FMT_DEFAULT << 8;
}

void puts(const char *s)
{
    while (*s != '\0')
        putchar(*s++);
    
    newLine();
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
        ((short*)VIDEO_MEMORY)[cursorPos++] = c | (FMT_DEFAULT << 8);

        updateCursor();
    }
}

void updateCursor()
{
    // TODO :
}
