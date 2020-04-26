#pragma once

#include <stddef.h>

// --- Memory --- //
void *memcpy(void *dest, const void *src, size_t n);

void *memmove(void *dest, const void *src, size_t n);

void *memset(void *str, int c, size_t n);

// --- String --- //
char *strcat(char *dest, const char *src);

int strcmp(const char *str1, const char *str2);

char *strcpy(char *dest, const char *src);

char *strdup(const char *s);

size_t strlen(const char *str);

char *strncpy(char *dest, const char *src, size_t n);

char *strtok(char *str, const char *delim);
