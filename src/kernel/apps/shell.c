#include "shell.h"

#include "drivers/screen.h"
#include "drivers/console.h"
#include <stdio.h>
#include <stdbool.h>

#define CMD_MAX_SIZE (SCREEN_WIDTH * 3)
#define CMD_MAX_ARGS 32

static unsigned int userInputBegin;

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

    // Init message
    puts("Os 2020");

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
        consoleDel();

    return possible;
}

void shellValidCommand()
{
    char cmd[CMD_MAX_SIZE];
    gets(cmd);

    consoleNewLine();

    shellEval(cmd);

    shellPS1();
}

// TODO : Update
void exec(const char *app, int argc, char **argv)
{
    printf("Execute <%s> with args { ", app);

    for (int i = 0; i < argc; ++i)
    {
        printf("%s", argv[i]);

        if (i != argc - 1)
            printf(", ");
    }

    puts(" }");
}

void shellEval(const char *cmd)
{
    // TODO : When fs, add cwd to argv and start argc to 1 in token loop
    char *argv[CMD_MAX_ARGS];

    const char *delim = " ";

    // App name
    char *token = strtok(cmd, delim);

    char *appName = token;

    int argc = 0;
    for ( ; token = strtok(NULL, delim); ++argc)
    {
        argv[argc] = token;
    }

    // TODO : Change exec
    exec(appName, argc, argv);
}
