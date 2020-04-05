#include "shell.h"

#include "drivers/screen.h"
#include "drivers/console.h"
#include <stdio.h>
#include <stdbool.h>

#define CMD_MAX_SIZE (SCREEN_WIDTH * 3)

static int userInputBegin;

// Moves the begining of user input
static void resetUserInput()
{
    userInputBegin = getCaret();

    // TODO : stdin flush
}

void shellInit()
{
    // Init display
    fillScreen('\0', (FMT_BLACK << 4) | FMT_GRAY);
    setCaret(0, 0);

    // PS1
    shellPS1();
}

void shellPS1()
{
    printf("> ");

    resetUserInput();
}

bool shellDelete()
{
    // Only if possible
    bool possible = getCaret() > userInputBegin;

    if (possible)
        // TODO : Remove stdin char
        consoleDel();

    return possible;
}

void shellValidCommand()
{
    // TODO : Parse command
    char cmd[CMD_MAX_SIZE];

    gets(cmd);

    consoleNewLine();
    printf("Exec <%s>\n", cmd);

    shellPS1();
}
