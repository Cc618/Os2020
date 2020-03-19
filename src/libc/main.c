#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <assert.h>

// #include <_assert.h>
#include <_string.h>
#include <_stdlib.h>
#include <_stdio.h>

#include "vector.h"

// !!! This code presents intentional memory leaks

// Prints items in a vector
void printVectorI(Vector *v)
{
    printf("[ ");
    for (size_t i = 0; i < v->size; ++i)
        printf("%d ", *(int*)v->data[i]);
    
    puts("]");
}

void tst_vector()
{
    Vector *v1 = Vector_new();
    int *_1 = malloc(sizeof(int));
    *_1 = 1;
    int *_2 = malloc(sizeof(int));
    *_2 = 2;

    printVectorI(v1);

    Vector_add(v1, _1);
    Vector_add(v1, _2);
    printVectorI(v1);
    
    Vector_clear(v1);
    printVectorI(v1);

    Vector_del(v1);
}

void tst_assert()
{
    // assert(0);
    // _assert(0);
}

void tst_string()
{
    const char *s1 = "Hello",
        *s2 = "World",
        *s3 = "",
        *s4 = "!";

    int buf[2] = { 1, 2 };
    int cpy_buf[2];

    puts("> memcpy");
    _memcpy(cpy_buf, buf, 2 * sizeof(int));
    printf("%d %d\n", buf[0], buf[1]);
    printf("%d %d\n", cpy_buf[0], cpy_buf[1]);

    puts("> memset");
    _memset(cpy_buf, 0, 2 * sizeof(int));
    puts("0 0");
    printf("%d %d\n", cpy_buf[0], cpy_buf[1]);

    puts("> strcat");
    char *cat_s = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(cat_s, s1);
    strcat(cat_s, s2);
    printf("%s\n", cat_s);
    printf("HelloWorld\n");

    puts("> strcmp");
    printf("%d %d %d %d\n", _strcmp(s1, s1), _strcmp(s1, s2), _strcmp(s2, s3), _strcmp(s3, s2));
    printf("%d %d %d %d\n", strcmp(s1, s1), strcmp(s1, s2), strcmp(s2, s3), strcmp(s3, s2));

    puts("> strcpy");
    char *cpy_s1 = _malloc(_strlen(s1) + 1);
    _strcpy(cpy_s1, s1);
    printf("%s\n", cpy_s1);
    printf("%s\n", s1);

    puts("> strdup");
    char *dup_s1 = _strdup(s1);
    printf("%s\n", dup_s1);
    printf("%s\n", s1);

    puts("> strlen");
    printf("%d %d %d %d\n", _strlen(s1), _strlen(s2), _strlen(s3), _strlen(s4));

    puts("> strncpy");
    size_t s1_nlen = _strlen(s1);
    char *ncpy_s1 = _malloc(s1_nlen - 3);
    _strncpy(ncpy_s1, s1, s1_nlen - 3);
    printf("%s\n", ncpy_s1);
    printf("%s\n", strncpy(memset(ncpy_s1, 0, s1_nlen), s1, s1_nlen - 3));
}

void tst_stdio()
{
    puts("> printf");
    _printf("");
    printf("");
    _printf("Hello\n");
    printf("Hello\n");
    _printf("%d\n", 3);
    printf("%d\n", 3);
    _printf("%d\n", 398201838);
    printf("%d\n", 398201838);
    _printf("%s %d %d %X\n", "nb:", 2000000000, -64, 0xCC05);
    printf("%s %d %d %X\n", "nb:", 2000000000, -64, 0xCC05);
}
#include <stdint.h>
int main()
{
    puts("--- Kernel ---");
    // tst_vector();

    puts("--- Libc ---");
    // tst_assert();
    // tst_string();
    tst_stdio();

    return 0;
}
