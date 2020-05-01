#include "context.h"

#include <string.h>

Context *Context_new(const char *cwd)
{
    Context *c = malloc(sizeof(Context));

    c->cwd = strdup(cwd);

    return c;
}

void Context_del(Context *c)
{
    free(c->cwd);
    free(c);
}
