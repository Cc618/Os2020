#include "string.h"

#include "k/syscalls.h"
#include <stdlib.h>
#include <stdbool.h>

char *strcat(char *dest, const char *src)
{
    char *dst = dest;

    // Go to the end of dest
    while (*dest != '\0')
        ++dest;

    strcpy(dest, src);

    return dst;
}

int strcmp(const char *a, const char *b)
{
    while (*a != '\0' && *a == *b)
    {
        ++a;
        ++b;
    }

    if (*a < *b)
        return -1;

    if (*a > *b)
        return 1;

    return 0;
}

char *strcpy(char *dest, const char *src)
{
    char *dst = dest;

    while (*src != '\0')
    {
        // Set memory
        *dest = *src;

        // Update pointers
        ++dest;
        ++src;
    }

    // Terminate
    *dest = '\0';

    return dst;
}

char *strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *dup = malloc(len);

    // Can't allocate
    if (!dup)
        return NULL;

    // Set data
    strcpy(dup, s);

    return dup;
}

size_t strlen(const char *s)
{
    size_t len = 0;
    while (*(s + len) != '\0')
        ++len;

    return len;
}

char *strncpy(char *dest, const char *src, size_t n)
{
    size_t i = 0;
    while (i < n && src[i] != '\0')
    {
        // Set memory
        dest[i] = src[i];

        ++i;
    }

    // Zero the tail
    while (i <= n)
    {
        dest[i] = '\0';
        ++i;
    }

    return dest;
}

char *strtok(char *str, const char *delim)
{
    // Start of the token if str is NULL
    static char *last;

    if (!str)
    {
        if (!last)
            return NULL;

        str = last;
    }

    // End of token
    if (*str == '\0')
    {
        last = NULL;
        return NULL;
    }

    // Save str
    char *s = str;

    // Delimit
    size_t delimLen = strlen(delim);

    // To cut multiple tokens in a row
    bool wasInToken = false;
    bool inToken = false;
    while (!(wasInToken && !inToken))
    {
        wasInToken = inToken;
        inToken = false;

        // Last token
        if (*str == '\0')
        {
            last = NULL;
            return s;
        }

        for (size_t i = 0; i < delimLen; ++i)
            if (*str == delim[i])
            {
                // Cut
                *str = '\0';

                // Save str
                last = str + 1;

                inToken = true;
            }

        // Next char
        ++str;
    }

    return s;
}
