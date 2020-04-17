# Changes

## 0.2

- Added color app to change the color of the tty
- Added Virtual File System (vfs)
- Added Fat32 driver
- Moved libc and kernel/k to lib
- Remade syscalls
- Added data structures like vector to k

## 0.1

This update is the first beta release of Os 2020.

Contains a second stage bootloader with a kernel which contains a minimal libc.
The libc provides main macros and IO functions.
There is the shell app providing some builtins : exit, echo and cat.
Some interrupts are implemented and there are syscalls.
Debug mode works fine with QEMU / GDB.
