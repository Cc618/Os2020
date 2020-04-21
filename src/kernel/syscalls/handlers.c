#include "syscalls/syscalls.h"

#include "drivers/screen.h"
#include "io/file.h"
#include "io/pipe.h"
#include <k/vector.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FMT_FATAL ((FMT_LIGHT_BLUE << 4) | FMT_WHITE)

void terminateKernel();

// --- System --- //
// Gathers the stack state to terminate a process
// when pressing Ctrl + C or sending termination signal
Vector *stackStates;

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

size_t sys_read(fd_t fd, void *buffer, size_t count)
{
    File *f = getFile(fd);

    // Not found
    if (!f)
        return 0;

    return File_read(f, buffer, count);
}

size_t sys_write(fd_t fd, void *buffer, size_t count)
{
    File *f = getFile(fd);

    // Not found
    if (!f)
        return 0;

    return File_write(f, buffer, count);
}

void sys_close(fd_t fd)
{
    File *f = getFile(fd);

    if (f != NULL)
        File_close(f);
}

fd_t sys_pipe()
{
    File *f = Pipe_new();

    return f->fd;
}
