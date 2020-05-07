#include "context.h"

#include "io.h"
#include <string.h>

Context *Context_new(const char *cwd)
{
    Context *c = malloc(sizeof(Context));

    c->cwd = strdup(cwd);

    c->stdin = STDIN_FD;
    c->stdout = STDOUT_FD;
    c->stderr = STDERR_FD;

    return c;
}

void Context_del(Context *c)
{
    free((char*)c->cwd);
    free(c);
}
