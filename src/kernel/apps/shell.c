#include "shell.h"

#include "drivers/screen.h"
#include "drivers/console.h"
#include "app.h"
#include "cat.h"
#include "echo.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CMD_MAX_SIZE (SCREEN_WIDTH * 3)
#define CMD_MAX_ARGS 32

#define BUILTIN_NOT_FOUND 0x7F100001

static unsigned int userInputBegin;
static bool shellRunning;
static int shellExitCode;

// Root by default
static const char *shellCwd = "/";

// Moves the begining of user input
static void resetUserInput()
{
    userInputBegin = getCaret();

    // TODO : stdin flush
}

static int shellExit(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
    shellRunning = false;

    // if (argc == 2)
    // {
    //     // TODO : atoi to return code
    // }

    return 0;
}

// Tries to executes a builtin command
// Returns the exit code of the command
// or BUILTIN_NOT_FOUND if no builtin found
static int tryExecBuiltin(const char *app, int argc, char **argv)
{
    if (strcmp(app, "echo") == 0)
        return execApp(echo, argc, argv);
    
    if (strcmp(app, "cat") == 0)
        return execApp(cat, argc, argv);
    
    if (strcmp(app, "color") == 0)
        return execApp(colorMain, argc, argv);
    
    if (strcmp(app, "exit") == 0)
        return shellExit(argc, argv);
    
    return BUILTIN_NOT_FOUND;
}

int shellMain(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
    shellRunning = true;
    shellExitCode = 0;

    // Init display
    fillScreen('\0', (FMT_BLACK << 4) | FMT_GRAY);
    setCaret(0, 0);

    // Init message
    puts("Os 2020");

    char cmd[CMD_MAX_SIZE];
    while (shellRunning)
    {
        // PS1
        shellPS1();

        // Get input
        gets(cmd);

        // Evaluate command
        shellEval(cmd);
    }

    return shellExitCode;
}

void shellPS1()
{
    // TODO : Push / pop format to have console format
    printf("@ %s > ", shellCwd);

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

void shellEval(const char *CMD)
{
    char *cmd = strdup(CMD);

    char *argv[CMD_MAX_ARGS];
    argv[0] = shellCwd;

    const char *delim = " ";

    // App name
    char *token = strtok(cmd, delim);
    char *appName = token;

    // Retrieve arguments
    int argc = 1;
    for ( ; (token = strtok(NULL, delim)); ++argc)
        argv[argc] = token;

    // Execute command
    int ret = tryExecBuiltin(appName, argc, argv);

    if (ret == BUILTIN_NOT_FOUND)
    {
        ret = exec(appName, argc, argv);

        if (ret != 0)
            printf("App exits with code %d\n", ret);
    }
}
