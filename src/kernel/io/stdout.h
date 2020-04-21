#pragma once

#include "file.h"

// Like a new but this is a static instance
void Stdout_init();

size_t Stdout_write(File *f, void *buffer, size_t count);
