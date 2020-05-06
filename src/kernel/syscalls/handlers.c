#include "syscalls/syscalls.h"

#include "drivers/screen.h"
#include "io/file.h"
#include "io/fs_file.h"
#include "io/pipe.h"
#include <k/vector.h>
#include <k/io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FMT_FATAL ((FMT_LIGHT_BLUE << 4) | FMT_WHITE)

void terminateKernel();

// Defined in handler.asm
extern int sys_enterProcess(int (*entry)(int argc, char **argv), int argc, char **argv);

// --- System --- //
// Gathers the stack state to terminate a process
// when pressing Ctrl + C or sending termination signal
Vector *stackStates;

// Like stackStates but for app contexts
Vector *appContexts = NULL;

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

int sys_enter(Context *c, int (*entry)(int argc, char **argv), int argc, char **argv)
{
    // Push context
    Vector_add(appContexts, c);

    int ret = sys_enterProcess(entry, argc, argv);

    // Pop and free context
    free(Vector_pop(appContexts));

    return ret;
}

// --- IO --- //
fd_t sys_open(const char *path, u8 mode)
{
    File *f = FSFile_new(path, mode);

    // Error
    if (f == NULL)
        return -1;

    return f->fd;
}

ssize_t sys_read(fd_t fd, void *buffer, size_t count)
{
    // Context redirections
    switch (fd)
    {
    case STDIN_FD:
        fd = currentContext()->stdin;
        break;
    case STDOUT_FD:
        fd = currentContext()->stdout;
        break;
    case STDERR_FD:
        fd = currentContext()->stderr;
        break;
    }

    File *f = getFile(fd);

    // Not found
    if (!f)
        return -1;

    return File_read(f, buffer, count);
}

ssize_t sys_write(fd_t fd, void *buffer, size_t count)
{
    // Context redirections
    switch (fd)
    {
    case STDIN_FD:
        fd = currentContext()->stdin;
        break;
    case STDOUT_FD:
        fd = currentContext()->stdout;
        break;
    case STDERR_FD:
        fd = currentContext()->stderr;
        break;
    }
    
    File *f = getFile(fd);

    // Not found
    if (!f)
        return -1;

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

// --- Files --- //
char **sys_ls(const char *dir, size_t *outCount)
{
    char *path = absPath(currentContext(), dir);
    FSEntry *f = getEntry(path);
    free(path);

    // Not found
    if (!f)
        return NULL;

    FSEntry **files = FSEntry_list(f, outCount);

    free(f);

    // Not a directory
    if (!files)
        return NULL;

    char **children = malloc(sizeof(char*) * *outCount);

    for (size_t i = 0; i < *outCount; ++i)
        children[i] = strdup(files[i]->name);

    return children;
}

Context *sys_context()
{
    Context *c = malloc(sizeof(Context));

    memcpy(c, currentContext(), sizeof(Context));

    return c;
}

void sys_touch(const char *p, bool directory)
{
    char *path = absPath(currentContext(), p);
    char *dir;
    char *name;
    
    cutPath(path, &dir, &name);

    FSEntry *f = getEntry(dir);

    if (f == NULL)
        goto end;

    FSEntry_touch(f, name, directory ? FS_DIRECTORY : 0);

    FSEntry_del(f);

end:;
    free(dir);
    free(name);
    free(path);
}

FInfo *sys_finfo(const char *path)
{
    FInfo *info = malloc(sizeof(FInfo));

    FSEntry *f = getEntry(path);

    if (!f)
    {
        free(info);
        info = NULL;
    }
    else
    {
        info->directory = f->flags & FS_DIRECTORY;
        info->size = f->size;
        FSEntry_del(f);
    }

    return info;
}
