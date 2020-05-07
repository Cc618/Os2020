#pragma once

#include "file.h"

// Like stdout but writes in red

void Stderr_init();

ssize_t Stderr_write(File *f, void *buffer, size_t count);
