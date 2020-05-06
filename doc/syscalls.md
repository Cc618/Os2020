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
#### open
- Opens a file at the absolute path 'path'
```c
fd_t open(const char *path);
```

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

### Files
#### ls
- List all files within dir
```c
char **ls(const char *dir, size_t *outCount);
```

#### context
- Returns a soft copy of the current context
```c
Context *context();
```

#### touch
- Creates a new file / directory
```c
void touch(const char *path, bool directory);
```

#### finfo
- Returns properties of the file
```c
FInfo *finfo(const char *path);
```

## How to add a syscall

Here is what to update to add a new syscall named **ID** with **NARG** arguments :

- lib/libk/k/syscalls.h : Add the define SYS_**ID** with a new value and the extern prototype **ID**
- lib/libk/k/syscalls.inc : Add the same define as above with the same value
- lib/libk/k/syscalls.asm : Add the declaration DEF_SYSC_**NARG** **ID**, SYS_**ID**
- kernel/syscalls/syscall_request.c : Add the entry [SYS_**ID**] = sys_**ID** to the syscalls array
- kernel/syscalls/syscalls.h : Add the function prototype sys_**ID**
- This file : Update the documentation
