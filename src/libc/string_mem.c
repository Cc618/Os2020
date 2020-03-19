#include <_string.h>

void *_memcpy(void *dest, const void *src, size_t n)
{
    while (n != 0)
    {
        --n;

        // Set memory
        ((char*)dest)[n] = ((const char*)src)[n];
    }

    return dest;
}

void *_memset(void *str, int c, size_t n)
{
    while (n != 0)
    {
        --n;

        // Set memory
        ((char*)str)[n] = (char)c;
    }

    return str;
}

