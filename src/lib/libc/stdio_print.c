#include "stdio.h"

#include <k/syscalls.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Maximum power of 10 that can be stored
// in an int32_t
#define UINT32_10POW_MAX 1000000000

// --- Flags --- //
// Flags return the number of characters written
// %u flag
int printf_uint32(FILE *f, uint32_t n)
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
            putc('0' + digit, f);
            ++written;
        }

        // Update variables
        n = rest;
        mod /= 10;
    }

    return written;
}

// %x / %X / %p flag
int printf_hex(FILE *f, uint32_t n, bool upper, bool isP)
{
    if (n == 0)
    {
        if (isP)
        {
            fputs("(nil)", f);

            // Size of (nil) is 5
            return 5;
        }
        else
        {
            putc('0', f);

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
                putc('0' + digit, f);
            else
                putc((upper ? 'A' : 'a') + digit - 0xA, f);

            ++written;
        }

        // Divide by 16
        mask >>= 4;
        shift -= 4;
    }

    return written;
}

// %d flag
int printf_int32(FILE *f, int n)
{
    if (n == 0)
    {
        putc('0', f);
        return 1;
    }

    // Negative
    if (n < 0)
    {
        putc('-', f);
        return 1 + printf_uint32(f, -n);
    }

    return printf_uint32(f, n);
}

// %s flag
int printf_string(FILE *f, char *s)
{
    int i = 0;
    while (s[i] != '\0')
        putc(s[i++], f);

    return i;
}

// --- Functions --- //
int fprintf(FILE *f, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int ret = vfprintf(f, fmt, args);

    va_end(args);
    return ret;
}

int fputc(int c, FILE *f)
{
    size_t written = write(f->_fileno, &c, 1);

    // Error
    if (written != 1)
        return EOF;

    return c;
}

int fputs(const char *s, FILE *f)
{
    int ret = 0;
    while (*s != '\0')
    {
        if (fputc(*s, f) == EOF)
            return EOF;

        ++s;
        ++ret;
    }

    return ret;
}

int printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int ret = vfprintf(stdout, fmt, args);

    va_end(args);
    return ret;
}

int putchar(int c)
{
    putc(c, stdout);

    return 0;
}

int puts(const char *s)
{
    if (fputs(s, stdout) == EOF)
        return EOF;

    return putchar('\n');
}

int vfprintf(FILE *f, const char *fmt, va_list args)
{
    int written = 0;

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
                putc('%', f);
                ++written;
                break;

            case 's':
                written += printf_string(f, va_arg(args, char*));
                break;

            case 'd':
            case 'i':
                written += printf_int32(f, va_arg(args, int32_t));
                break;

            case 'c':
                ++written;
                putc(va_arg(args, int), f);
                break;

            case 'u':
                written += printf_uint32(f, va_arg(args, uint32_t));
                break;

            case 'x':
            case 'X':
            case 'p':
                written += printf_hex(f, va_arg(args, uint32_t), next == 'X', next == 'p');
                break;

            default:
                return -1;
            }
        }
            break;

        default:
            putc(c, f);
            ++written;
            break;
        }

        ++i;
    }

    return written;
}
