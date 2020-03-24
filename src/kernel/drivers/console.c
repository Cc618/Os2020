#include "console.h"

#include "screen.h"

// The char cursor
static unsigned int caret = 0;

void consolePut(char c)
{
    // TODO : FMT
    // TODO : CARET
    // TODO : CRLF...
    // TODO : End of screen

    unsigned int x = caret % SCREEN_WIDTH;
    setChar(x, caret / SCREEN_WIDTH, c, 0x0F);

    ++caret;
}
