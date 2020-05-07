#pragma once

#include "file.h"

void Stdout_init();

ssize_t Stdout_write(File *f, void *buffer, size_t count);
