#include "console.h"

#include "screen.h"

// The char cursor
static unsigned int caret = 0;

// Color format
uint8_t consoleFmt = FMT_DEFAULT;

// Update the caret (display)
static void updateCaret()
{
    unsigned int x = caret % SCREEN_WIDTH;
    unsigned int y = caret / SCREEN_WIDTH;
    setCaret(x, y);
}

void consolePut(char c)
{
    // TODO : FMT
    // TODO : CRLF...
    // TODO : End of screen

    if (c == '\n')
    {
        consoleNewLine();
        return;
    }

    unsigned int x = caret % SCREEN_WIDTH;
    setChar(x, caret / SCREEN_WIDTH, c, consoleFmt);

    ++caret;

    updateCaret();
}

void consoleNewLine()
{
    // TODO : Scroll
    caret += SCREEN_WIDTH;
    caret -= caret % SCREEN_WIDTH;

    updateCaret();
}
