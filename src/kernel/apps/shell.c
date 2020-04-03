#include "shell.h"

#include "drivers/screen.h"
#include "drivers/console.h"
#include <stdio.h>
#include <stdbool.h>

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

void shellDelete()
{
    // Only if possible
    if (getCaret() > userInputBegin)
        // TODO : Remove stdin char
        consoleDel();
}

void shellValidCommand()
{
    // TODO : Parse command
    char cmd[80];
    
    gets(cmd);

    printf("Exec <%s>\n", cmd);
    
    consoleNewLine();

    shellPS1();
}
