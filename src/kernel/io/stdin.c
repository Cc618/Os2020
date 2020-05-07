#include "stdin.h"

#include "pipe.h"

void Stdin_init()
{
    // Stdin is just a pipe
    Pipe_new();
}
