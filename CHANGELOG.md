# Changes

## 0.2

- Added Virtual File System (vfs)
- Added Fat32 driver
- Added the k library, syscalls + types...
- Remade syscalls
- Added data structures like vector and queue to k
- Added pipes in kernel
- Added app context : syscalls enter / context / terminate (Ctrl + C works)
- Updated stdio with main functions and implemented streams (FILE)
- Added color app to change the color of the tty
- Added several linux apps like cd, mkdir, ls, cat...
- Created shell app with input redirection
- Created a script to retrieve file system or to create a new one (on Makefile)

## 0.1

This update is the first beta release of Os 2020.

Contains a second stage bootloader with a kernel which contains a minimal libc.
The libc provides main macros and IO functions.
There is the shell app providing some builtins : exit, echo and cat.
Some interrupts are implemented and there are syscalls.
Debug mode works fine with QEMU / GDB.
