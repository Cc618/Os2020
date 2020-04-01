#include "stdstream.h"

#include "drivers/console.h"
#include "syscalls/syscalls.h"

// TODO : rm
// For GET_C syscall
// bool stdinBusy = false;

// static void stdinPush(__attribute__((unused)) Stream *stream, uint8_t *data, size_t count)
// {
//     for (size_t i = 0; i < count; ++i)
//     {
//         if (stdinBusy)
//         {
//             syscallRet = (int)data[i];
    
//             stdinBusy = false;
//         }

//         // TODO : Shell keyboard event
//         consolePut(data[i]);
//     }
// }

Stream stdinStream = {
    .push = Stream_pushDummy
};
