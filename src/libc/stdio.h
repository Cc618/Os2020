#pragma once

// Std streams
#define stdin 1
#define stdout 2
#define stderr 3

char *gets(char *s);

int getchar();

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
