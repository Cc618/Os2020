#include "console.h"

#include "screen.h"

// The char cursor
static unsigned int caret = 0;

void consolePut(char c)
{
    // TODO : FMT
    // TODO : CARET update
    // TODO : CRLF...
    // TODO : End of screen

    if (c == '\n')
    {
        consoleNewLine();
        return;
    }

    unsigned int x = caret % SCREEN_WIDTH;
    setChar(x, caret / SCREEN_WIDTH, c, 0x0F);

    ++caret;
}

void consoleNewLine()
{
    // TODO : Scroll
    caret += SCREEN_WIDTH;
    caret -= caret % SCREEN_WIDTH;
}

