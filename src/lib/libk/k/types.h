#pragma once

// System types
// TODO : Include this file instead of libc ones in kernel

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;

// File descriptor type
typedef u32 fd_t;
