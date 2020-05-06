#include "shell.h"

#include "drivers/screen.h"
#include "drivers/console.h"
#include "builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <k/syscalls.h>
#include <k/io.h>
#include <k/finfo.h>

#define CMD_MAX_SIZE (SCREEN_WIDTH * 3)
#define CMD_MAX_ARGS 32

#define BUILTIN_NOT_FOUND 0x7F100001

static unsigned int userInputBegin;
static bool shellRunning;
static int shellExitCode;

// !!! Root is represented by an empty string, not /
static char *shellCwd;

// Moves the begining of user input
static void resetUserInput()
{
    userInputBegin = getCaret();
}

static int shellExit(__attribute__((unused)) int argc, __attribute__((unused)) char **argv)
{
    shellRunning = false;

    return 0;
}

static int shellCd(int argc, char **argv)
{
    if (argc > 2)
    {
        fprintf(stderr, "cd: Too many args\n");
        return -1;
    }
    else if (argc == 1)
    {
        free(shellCwd);
        shellCwd = strdup("");
    }
    else // argc == 2
    {
        char *newCwd;

        if (strcmp(argv[1], "..") == 0)
        {
            // Root case
            if (shellCwd[0] == '\0')
                return 0;

            // Retrieve the last name
            newCwd = dirPath(shellCwd);
            free(shellCwd);
            shellCwd = newCwd;

            return 0;
        }

        // Absolute path
        if (argv[1][0] == '/')
            newCwd = strdup(argv[1]);
        else
        {
            size_t len = strlen(argv[1]);
            size_t cwdLen = strlen(shellCwd);

            // cwd/dir\0
            newCwd = malloc(cwdLen + 1 + len + 1);

            memcpy(newCwd, shellCwd, cwdLen);
            newCwd[cwdLen] = '/';
            memcpy(newCwd + cwdLen + 1, argv[1], len + 1);
        }

        FInfo *info = finfo(newCwd);
        if (info == NULL)
        {

            fprintf(stderr, "cd: '%s' not found\n", newCwd);
            return -1;
        }

        if (info->directory)
        {
            free(shellCwd);
            shellCwd = newCwd;
            free(info);
        }
        else
        {
            fprintf(stderr, "cd: '%s' is not a directory\n", newCwd);
            free(info);
            return -1;
        }
    }

    return 0;
}

// Tries to executes a builtin command
// Returns the exit code of the command
// or BUILTIN_NOT_FOUND if no builtin found
static int tryExecBuiltin(const char *app, int argc, char **argv)
{
    Context *ctxt = Context_new(shellCwd);

    // Static builtins
    if (strcmp(app, "exit") == 0)
        return shellExit(argc, argv);

    if (strcmp(app, "cd") == 0)
        return shellCd(argc, argv);

    // Builtins
    if (strcmp(app, "cat") == 0)
        return enter(ctxt, cat, argc, argv);

    if (strcmp(app, "color") == 0)
        return enter(ctxt, colorMain, argc, argv);

    if (strcmp(app, "echo") == 0)
    {
        // TMP
        ctxt->stdout = open("/dir/out", F_WRITE);

        int ret = enter(ctxt, echo, argc, argv);

        close(ctxt->stdout);

        return ret;
    }

    if (strcmp(app, "ls") == 0)
        return enter(ctxt, lsMain, argc, argv);

    Context_del(ctxt);

    return BUILTIN_NOT_FOUND;
}

int shellMain(int argc, char **argv)
{
    shellExitCode = 0;

    if (argc == 2)
    {
        // Display help
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        {
            puts("Usage : shell [<dir>='']");
            return 0;
        }

        shellCwd = absPath(context(), argv[1]);

        // If last char is /, remove it
        size_t len = strlen(shellCwd);
        if (shellCwd[len - 1] == '/')
            shellCwd[len - 1] = '\0';

        // Verify validity
        FInfo *info = finfo(shellCwd);

        if (info == NULL)
        {
            shellExitCode = -1;
            goto shellExit;
        }

        if (!info->directory)
        {
            shellExitCode = -1;
            free(info);
            goto shellExit;
        }

        free(info);
    }
    else
        // Root
        shellCwd = strdup("");

    shellRunning = true;

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

shellExit:;
    free(shellCwd);

    return shellExitCode;
}

void shellPS1()
{
    // TODO : Push / pop format to have console format

    printf("@ %s > ", shellCwd[0] == '\0' ? "/" : shellCwd);

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
    argv[0] = strdup(shellCwd);

    const char *delim = " \n";

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
        puts("No app found");

    // TODO : When apps : Exec
    // {
    //     ret = exec(appName, argc, argv);

    //     if (ret != 0)
    //         printf("App exits with code %d\n", ret);
    // }

    free(cmd);
    for (size_t i = 0; i < (size_t)argc; ++i)
        free(argv[i]);
}
