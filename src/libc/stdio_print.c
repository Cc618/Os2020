#include <_stdio.h>

#include <_stdlib.h>
#include <_stdbool.h>
#include <_stdint.h>

// TODO
#include <stdarg.h>
#include <stdio.h>

// Maximum power of 10 that can be stored
// in an int32_t
#define UINT32_10POW_MAX 1000000000

// --- Flags --- //
// Flags return the number of characters written
// %u flag
int print_uint32(uint32_t n)
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
            _putchar('0' + digit);
            ++written;
        }

        // Update variables
        n = rest;
        mod /= 10;
    }

    return written;
}

// %x / %X / %p flag
int print_hex(uint32_t n, bool upper, bool allowLeading0)
{
    int written = 0;
    // Whether we already have written digits (avoid leading 0s)
    bool started = false || allowLeading0;

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
                _putchar('0' + digit);
            else
                _putchar((upper ? 'A' : 'a') + digit - 0xA);

            ++written;
        }

        // Divide by 16
        mask >>= 4;
        shift -= 4;
    }

    return written;
}

// %d flag
int print_int32(int n)
{
    // Negative
    if (n < 0)
    {
        _putchar('-');
        return 1 + print_uint32(-n);
    }

    return print_uint32(n);
}

// %s flag
int print_string(char *s)
{
    int i = 0;
    while (s[i] != '\0')
        _putchar(s[i++]);
    
    return i;
}

// --- Functions --- //
int _printf(const char *fmt, ...)
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
                _putchar('%');
                ++written;
                break;

            case 's':
                written += print_string(va_arg(args, char*));
                break;

            case 'd':
            case 'i':
                written += print_int32(va_arg(args, int32_t));
                break;

            case 'c':
                ++written;
                _putchar(va_arg(args, int));
                break;

            case 'u':
                written += print_uint32(va_arg(args, uint32_t));
                break;

            case 'x':
            case 'X':
            case 'p':
                written += print_hex(va_arg(args, uint32_t), next == 'X', next == 'p');
                break;

            default:
                // return -1
                written = -1;
                goto ret;
            }
        }
            break;

        default:
            _putchar(c);
            ++written;
            break;
        }

        ++i;
    }

ret:
    va_end(args);

    return written;
}

int _putchar(int c)
{
    return putchar(c);
}
