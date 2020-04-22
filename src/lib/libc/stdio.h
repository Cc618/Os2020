#pragma once

#include <k/types.h>

#define EOF (-1)

typedef struct FILE_t
{
    // File descriptor
    fd_t _fileno;
} FILE;

// Std streams
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

int fputc(int c, FILE *stream);

char *gets(char *s);

int getchar();

#define putc fputc

int putchar(int c);

int puts(const char *s);

// Implemented flags :
// - %%
// - %d
// - %i
// - %u
// - %c
// - %x
// - %X
// - %p
int printf(const char *fmt, ...);
