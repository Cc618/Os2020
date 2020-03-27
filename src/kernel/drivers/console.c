#include "console.h"

#include "screen.h"

// The char cursor
static unsigned int caret = 0;

// Color format
uint8_t consoleFmt = FMT_DEFAULT;

// Update the caret (display)
static void updateCaret()
{
    setCaretOffset(caret);
}

void consolePut(char c)
{
    // TODO : FMT
    
    // Scroll if necessary
    if (caret == SCREEN_WIDTH * SCREEN_HEIGHT)
        consoleScroll();

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
    caret += SCREEN_WIDTH;
    caret -= caret % SCREEN_WIDTH;

    // Scroll if necessary
    if (caret == SCREEN_WIDTH * SCREEN_HEIGHT)
        consoleScroll();

    updateCaret();
}

void consoleScroll()
{
    screenScroll(SCREEN_WIDTH);

    if (caret > SCREEN_WIDTH)
        caret -= SCREEN_WIDTH;
    else
        caret = 0;

    updateCaret();
}
