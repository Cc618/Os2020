#pragma once

#include <_stddef.h>

// string_mem.c //
void *_memcpy(void *dest, const void *src, size_t n);

void *_memset(void *str, int c, size_t n);

// string_str.c //
char *_strcat(char *dest, const char *src);

char *_strdup(const char *s);

int _strcmp(const char *str1, const char *str2);

char *_strcpy(char *dest, const char *src);

size_t _strlen(const char *str);

char *_strncpy(char *dest, const char *src, size_t n);
