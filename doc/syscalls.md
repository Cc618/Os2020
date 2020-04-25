# Syscalls

Syscalls are the only way to call kernel functions.

To syscall, include k/syscalls.h and call the associated function (see list below).

If the syscall has to occur in kernel mode, call sys_**ID**, this function avoid using interrupts.

## Syscalls List

All syscalls IDs like SYS_**ID** in k/syscalls.h.

Here is the list of syscalls.

### System
#### fatal
- Fatal error
```c
void fatal(const char *msg);
```

#### enter
- Calls the entry of an application
```c
int enter(int (*entry)(int argc, char **argv), int argc, char **argv);
```

#### terminate
- Terminates the current program
```c
int terminate();
```

### IO
#### read
- Reads count bytes of the file associated to fd in buffer
- Returns how many bytes read
```c
ssize_t read(fd_t fd, void *buffer, size_t count);
```

#### write
- Writes count bytes of buffer in the file associated to fd
- Returns how many bytes written
```c
ssize_t write(fd_t fd, void *buffer, size_t count);
```

#### close
- Closes a file
```c
void close(fd_t fd);
```

#### pipe
- Creates a pipe
- Returns its file descriptor
```c
fd_t pipe();
```
