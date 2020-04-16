#include "stdio.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include "syscall.h"

// Maximum power of 10 that can be stored
// in an int32_t
#define UINT32_10POW_MAX 1000000000

// --- Flags --- //
// Flags return the number of characters written
// %u flag
int printf_uint32(uint32_t n)
{
    int written = 0;
    uint32_t mod = UINT32_10POW_MAX;
    // Whether we already have written digits (avoid leading 0s)
    bool started = false;

    while (mod != 0)
    {
        // n without its last digit
        uint32_t rest = n % mod;
        // The last digit
        uint32_t digit = (n - rest) / mod;

        if (!started && digit != 0)
            started = true;

        if (started)
        {
            // Display this digit
            putchar('0' + digit);
            ++written;
        }

        // Update variables
        n = rest;
        mod /= 10;
    }

    return written;
}

// %x / %X / %p flag
int printf_hex(uint32_t n, bool upper, bool isP)
{
    if (n == 0)
    {
        if (isP)
        {
            putchar('('); putchar('n'); putchar('i'); putchar('l'); putchar(')');

            // Size of (nil) is 5
            return 5;
        }
        else
        {
            putchar('0');

            return 1;
        }
    }

    int written = 0;
    // Whether we already have written digits (avoid leading 0s)
    bool started = false || isP;

    // Shift of n in bits
    unsigned int shift = 28;

    // Mask to retrieve the digit
    uint32_t mask = 0xF0000000;

    while (mask != 0)
    {
        // Retrieve the digit
        unsigned int digit = (n & mask) >> shift;
        // Remove the digit
        n = n & (~mask);

        if (!started && digit != 0)
            started = true;

        // Display
        if (started)
        {
            if (digit < 0xA)
                putchar('0' + digit);
            else
                putchar((upper ? 'A' : 'a') + digit - 0xA);

            ++written;
        }

        // Divide by 16
        mask >>= 4;
        shift -= 4;
    }

    return written;
}

// %d flag
int printf_int32(int n)
{
    if (n == 0)
    {
        putchar('0');
        return 1;
    }

    // Negative
    if (n < 0)
    {
        putchar('-');
        return 1 + printf_uint32(-n);
    }

    return printf_uint32(n);
}

// %s flag
int printf_string(char *s)
{
    int i = 0;
    while (s[i] != '\0')
        putchar(s[i++]);

    return i;
}

// --- Functions --- //
int printf(const char *fmt, ...)
{
    int written = 0;

    va_list args;
    va_start(args, fmt);

    for (size_t i = 0; fmt[i] != '\0'; )
    {
        char c = fmt[i];
        switch (c)
        {
        case '%':
        {
            ++written;
            char next = fmt[++i];

            switch (next)
            {
            case '%':
                putchar('%');
                ++written;
                break;

            case 's':
                written += printf_string(va_arg(args, char*));
                break;

            case 'd':
            case 'i':
                written += printf_int32(va_arg(args, int32_t));
                break;

            case 'c':
                ++written;
                putchar(va_arg(args, int));
                break;

            case 'u':
                written += printf_uint32(va_arg(args, uint32_t));
                break;

            case 'x':
            case 'X':
            case 'p':
                written += printf_hex(va_arg(args, uint32_t), next == 'X', next == 'p');
                break;

            default:
                // return -1
                written = -1;
                goto ret;
            }
        }
            break;

        default:
            putchar(c);
            ++written;
            break;
        }

        ++i;
    }

ret:
    va_end(args);

    return written;
}

int putchar(int c)
{
    putc(c, stdout);

    return 0;
}

int puts(const char *s)
{
    // TODO : Use fputs with CRLF

    while (*s != '\0')
    {
        int status;
        if ((status = putchar(*s)))
            return status;

        ++s;
    }

    return putchar('\n');
}
