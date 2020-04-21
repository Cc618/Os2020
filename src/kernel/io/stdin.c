#include "stdin.h"

#include "pipe.h"

void Stdin_init()
{
    // Stdin is just a pipe
    File *f = Pipe_new();
}
