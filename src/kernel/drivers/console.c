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
    if (c == '\n')
    {
        consoleNewLine();
        return;
    }

    // Scroll if necessary
    if (caret >= SCREEN_WIDTH * SCREEN_HEIGHT)
        consoleScroll();

    setCharOffset(caret, c, consoleFmt);

    ++caret;

    updateCaret();
}

void consoleDel()
{
    if (caret % SCREEN_WIDTH == 0)
        return;

    --caret;

    uint8_t fmt = getScreenData(caret) >> 8;
    setCharOffset(caret, '\0', fmt);

    updateCaret();
}

void consoleNewLine()
{
    caret += SCREEN_WIDTH;
    caret -= caret % SCREEN_WIDTH;

    // Scroll if necessary
    if (caret >= SCREEN_WIDTH * SCREEN_HEIGHT)
        consoleScroll();
    else
        updateCaret();
}

void consoleScroll()
{
    screenScroll(SCREEN_WIDTH);

    if (caret >= SCREEN_WIDTH * SCREEN_HEIGHT)
        caret = SCREEN_WIDTH * (SCREEN_HEIGHT - 1);
    else if (caret > SCREEN_WIDTH)
        caret -= SCREEN_WIDTH;
    else
        caret = 0;

    updateCaret();
}

unsigned int getCaret()
{
    return caret;
}
