# Syscalls

Syscalls are the only way to call kernel functions.

To syscall, include syscalls/syscalls.h

## Syscalls Table

All syscalls are named like SYS_**ID**.

Here is the list of syscalls.

| ID | Arg1 | Arg2 | Arg3 | Arg4 | Description |
| -- | ---- | ---- | ---- | ---- | ----------- |
| EXIT |  |  |  |  | Powers off the computer |
| PUTC | c | fd |  |  | Puts the character c at the end of the file with file descriptor fd |
| STRCON | fd | cb |  |  | Connects the stream to the callback cb, functor (*)(FILE *f, uint8_t *data, size_t count) |

