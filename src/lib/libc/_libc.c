#include "_libc.h"

#include <stdio.h>
#include <k/syscalls.h>

void __libc_init()
{
    // --- stdio --- //
    // Std streams
    stdin = malloc(sizeof(FILE));
    stdout = malloc(sizeof(FILE));
    stderr = malloc(sizeof(FILE));

    stdin->_fileno = 0;
    stdout->_fileno = 1;
    stderr->_fileno = 2;
}
