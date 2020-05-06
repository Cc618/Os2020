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
// TMP : Update doc + absPath remove context
#define absPath(c, p) absPathFrom((c)->cwd, p);
// char *absPath(Context *c, const char *p);
char *absPathFrom(const char *cwd, const char *p);

// Returns the parent directory (without / at the end)
// * p is absolute or already has a parent directory (contains /)
// * The return has the same capacity of p
char *dirPath(const char *p);

// Retrieves the directory and name component of the path
// * p is absolute or already has a parent directory (contains /)
void cutPath(const char *p, char **outDir, char **outName);

// Replaces the last / by a null char
// * Returns the location of the string after the null char (the name)
char *inplaceCutPath(char *s);
