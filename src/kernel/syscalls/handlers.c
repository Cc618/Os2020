#include "syscalls/syscalls.h"

#include "drivers/screen.h"
#include <k/vector.h>
#include <k/regs.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FMT_FATAL ((FMT_LIGHT_BLUE << 4) | FMT_WHITE)

void terminateKernel();

// --- System --- //
// The location to return when there is a termination signal
void *terminationReturn;

// Gathers the stack state to return to onTerminate
// when pressing Ctrl + C or sending termination signal
static Vector *stackStates;

void sys_fatal(const char *msg)
{
    static const char *HEADER = "Fatal error: ";

    fillScreen('\0', FMT_FATAL);

    // Cat HEADER + msg
    size_t headerCount = strlen(HEADER);
    size_t count = headerCount + strlen(msg);
    char *errorMsg = malloc(count + 1);
    strcpy(errorMsg, HEADER);
    strcpy(errorMsg + headerCount, msg);
    errorMsg[count] = '\0';

    for (int x = 0; *errorMsg != '\0'; )
    {
        setChar(x, 0, *errorMsg, FMT_FATAL);

        ++x;
        ++errorMsg;
    }

    free(errorMsg);

    // Disable computer
    terminateKernel();
}

int sys_enter(int (*entry)(int argc, char **argv), int argc, char **argv)
{
    // Prepare global variables
    terminationReturn = &&onTerminate;
    if (!stackStates)
        stackStates = Vector_new();

    int ret;

    // Push the new state
    StackRegs *state = malloc(sizeof(StackRegs));
    STACK_REGS_GET(state);
    Vector_add(stackStates, state);

    // TMP
    // TODO : Disable interrupts
    // Execute the app
    ret = entry(argc, argv);

    // Retrieve this state because there is no
    // termination signal
    free(Vector_pop(stackStates));

    goto onExit;

    // Ctrl + C termination
onTerminate:;
    ret = -2;
    goto onExit;

onExit:;
    return ret;
}

// --- IO --- //
void sys_putc(u8 c, int fd)
{
    // TODO : Use table
    switch (fd)
    {
    case stdout:
        // TODO : Implement
        stdoutStream.push(&stdoutStream, &c, 1);
        break;

    case stderr:
        stderrStream.push(&stderrStream, &c, 1);
        break;

    case stdin:
        stdinStream.push(&stdinStream, &c, 1);
        break;
    
    default:
        // TODO : When fs, write to file

        sys_fatal("No file associated with this descriptor");
        break;
    }
}

void sys_strcon(int fd, void (*cb)(Stream *f, u8 *data, size_t count))
{

    // TODO : Table
    switch (fd)
    {
    case stdin:
        stdinStream.push = cb;
        break;
    
    case stdout:
        stdoutStream.push = cb;
        break;

    case stderr:
        stderrStream.push = cb;
        break;
    
    default:
        sys_fatal("No file associated with this descriptor");
        break;
    }
}

