#include "ctype.h"

int isalnum(char c)
{
    return (c >= 0x41 && c <= 0x5A)     // isupper
        || (c >= 0x61 && c <= 0x7A)     // islower
        || (c >= 0x30 && c <= 0x39);    // isdigit
}

int isalpha(char c)
{
    return (c >= 0x41 && c <= 0x5A)     // isupper
        || (c >= 0x61 && c <= 0x7A);    // islower
}

int isblank(char c)
{
    return c == 0x09
        || c == 0x20;
}

int iscntrl(char c)
{
    return c <= 0x1F
        || c == 0x7F;
}

int isdigit(char c)
{
    return c >= 0x30 && c <= 0x39;
}

int isgraph(char c)
{
    return c >= 0x21 && c <= 0x7E;
}

int islower(char c)
{
    return c >= 0x61 && c <= 0x7A;
}

int isprint(char c)
{
    return c >= 0x20 && c <= 0x7E;
}

int ispunct(char c)
{
    return (c >= 0x21 && c <= 0x2F)
        || (c >= 0x3A && c <= 0x40)
        || (c >= 0x5B && c <= 0x60)
        || (c >= 0x7B && c <= 0x7E);
}

int isspace(char c)
{
    return (c >= 0x09 && c <= 0x0D)
        ||  c == 0x20;
}

int isupper(char c)
{
    return c >= 0x41 && c <= 0x5A;
}

int isxdigit(char c)
{
    return (c >= 0x30 && c <= 0x39)
        || (c >= 0x41 && c <= 0x46)
        || (c >= 0x61 && c <= 0x66);
}

char tolower(char c)
{
    if (c >= 0x41 && c <= 0x5A)
        c += 0x20;
    return c;
}

char toupper(char c)
{
    if (c >= 0x61 && c <= 0x7A)
        c -= 0x20;
    return c;
}
