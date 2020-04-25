# Lib

Os2020 provides two standard libraries, libc and libk.
All files are located in src/lib.

## Libk

To include libk files use k/*.c, libk provides system related functions like syscalls and also data structures.

## Libc

libc is the traditionnal C standard library, not every function is implemented, see implemented functions above.

In addition, there is some definitions / structs like NULL, stdint.h and stdarg.h are also implemented.

### string.h

- memcpy
- memmove
- memset
- strcat
- strdup
- strcmp
- strcpy
- strlen
- strncpy
- strtok

### stdlib.h

- free
- malloc

### stdio.h

- fclose
- fgetc
- fgets
- fopen
- fprintf
- fputc
- fputs
- gets
- getchar
- putc
- putchar
- puts
- printf (partially, see implemented flags in stdio.h)
- vfprintf

### ctype.h

- isalnum
- isalpha
- isblank
- iscntrl
- isdigit
- isgraph
- islower
- isprint
- ispunct
- isspace
- isupper
- isxdigit
- tolower
- toupper
