#include "stdstream.h"

#include "drivers/console.h"
#include "drivers/screen.h"

extern void Stream_pushDummy(__attribute__((unused)) Stream *stream,
                            __attribute__((unused)) uint8_t *data,
                            __attribute__((unused)) size_t count)
{}

static void stdoutPush(__attribute__((unused)) Stream *_, uint8_t *data, size_t count)
{
    for (size_t i = 0; i < count; ++i)
        consolePut(data[i]);
}

static void stderrPush(__attribute__((unused)) Stream *_, uint8_t *data, size_t count)
{
    uint8_t lastFmt = consoleFmt;
    consoleFmt = FMT_ERROR;

    for (size_t i = 0; i < count; ++i)
        consolePut(data[i]);

    consoleFmt = lastFmt;
}

Stream stdoutStream = {
    .push = stdoutPush
};

Stream stderrStream = {
    .push = stderrPush
};
