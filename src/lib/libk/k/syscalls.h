#pragma once

// All syscalls (user)

#include "types.h"
#include "context.h"

// --- Id --- //
// !!! These definitions should be the same as syscalls.inc's definitions
// Syscalls ids
// See doc/syscalls.md for specs
#define SYS_FATAL       0x02
#define SYS_ENTER       0x03
#define SYS_TERMINATE   0x04

#define SYS_OPEN        0x10
#define SYS_READ        0x11
#define SYS_WRITE       0x12
#define SYS_CLOSE       0x13
#define SYS_PIPE        0x14

#define SYS_LS          0x20
#define SYS_CONTEXT     0x21
#define SYS_TOUCH       0x22
#define SYS_FINFO       0x23


// --- System --- //
// Fatal error
extern void fatal(const char *msg);

// Calls the entry of an application
// The context is auto freed
extern int enter(Context *c, int (*entry)(int argc, char **argv), int argc, char **argv);

// Terminates the current program
extern int terminate();

// --- IO --- //
// Opens a file
// * Modes are described in k/io.h
// * Returns -1 on error
extern fd_t open(const char *path, u8 mode);

// Reads count bytes of the file associated to fd in buffer
// * Returns how many bytes read
extern ssize_t read(fd_t fd, void *buffer, size_t count);

// Writes count bytes of buffer in the file associated to fd
// * Returns how many bytes written
extern ssize_t write(fd_t fd, void *buffer, size_t count);

// Closes a file
extern void close(fd_t fd);

// Creates a pipe
// * Returns its file descriptor
extern fd_t pipe();

// --- Files --- //
// List all files within dir
// - outCount : Number of items
// * Returns NULL if dir invalid
// * Retrieve only the names, not paths
extern char **ls(const char *dir, size_t *outCount);

// Returns a soft copy of the current context
// * Use currentContext() in kernel mode instead
// * Only free the context, don't use Context_del since the copy is soft
extern Context *context();

// Creates a new file / directory
// * Returns whether the file has been created
// * The file must have a parent directory
// * If the file already exists, nothing is done
extern void touch(const char *path, bool directory);

// Returns properties of the file
// * Returns NULL if path doesn't exist
extern FInfo *finfo(const char *path);
