#pragma once

// Std streams
#define stdin 0
#define stdout 1
#define stderr 2

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
