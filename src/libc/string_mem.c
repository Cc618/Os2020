#include <string.h>

#include <stdint.h>

void *memcpy(void *dest, const void *src, size_t n)
{
    while (n != 0)
    {
        --n;

        // Set memory
        ((char*)dest)[n] = ((const char*)src)[n];
    }

    return dest;
}

void* memmove(void *dest, const void *src, size_t count)
{
    if (count == 0)
        return NULL;

    if (dest > src)
    {
        size_t i = count;

        while (1)
        {
            --i;

            ((uint8_t*) dest)[i] = ((uint8_t*) src)[i];

            if (i == 0)
                break;
        }
    }
    else for (size_t i = 0; i < count; ++i)
        ((uint8_t*) dest)[i] = ((uint8_t*) src)[i];

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

