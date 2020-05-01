#pragma once

#include "context.h"

#define INVALID_FD (-1)
#define STDIN_FD 0
#define STDOUT_FD 1
#define STDERR_FD 2

// Open modes
#define F_READ      1
#define F_WRITE     2
#define F_APPEND    4

// Generates the absolute path to p considering
// the context c (usually the current app context)
// * p can be a file, a directory, empty or ./..
// * p can also be absolute, a duplicate is returned
char *absPath(Context *c, const char *p);
