#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Streams related functions, a stream is almost like a file in linux

typedef struct stream_t {
    // When we append 'count' bytes of data to the stream
    void (*push)(struct stream_t *stream, uint8_t *data, size_t count);
} Stream;

// --- stdouts --- //
extern Stream stdoutStream;
extern Stream stderrStream;
extern Stream stdinStream;

// extern bool stdinBusy;

// Does nothing, used to stop streams
// Defined in stdstream_stdout.c
extern void Stream_pushDummy(__attribute__((unused)) Stream *stream, uint8_t *data, size_t count);

