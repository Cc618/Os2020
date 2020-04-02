# Libc

The libc doesn't contain all functions, here is the list of implemented functions.

## Notes

In addition, there is some definitions like NULL, int types and stdarg.h is implemented.

The syscall system is different in Os 2020, we can call a syscall by calling the function syscall(), syscall doesn't have any argument, we use macros SYSC0 -> SYSC4 to set arguments. Arguments are set in syscallArg1 -> syscallArg4 global variables.

## string.h

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

## stdlib.h

- free
- malloc

## stdio.h

- gets
- putchar
- puts
- printf (partially, see implemented flags in stdio.h)

## ctype.h

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
