#pragma once

// A pipe is a file represented by a FIFO in kernel space
// !!! A pipe has a fixed size : PIPE_CAPACITY

#include "file.h"

// How many bytes within a pipe
#define PIPE_CAPACITY 4096

// Creates a new pipe and register it
File *Pipe_new();

// Returns file ops for a pipe
FileOps *Pipe_ops();

void Pipe_del(File *f);

size_t Pipe_read(File *f, void *buffer, size_t count);

size_t Pipe_write(File *f, void *buffer, size_t count);
