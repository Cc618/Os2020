#include "irq_handlers.h"

#include "drivers/ports.h"
#include "drivers/keyboard.h"
#include "syscalls/syscalls.h"

#define INT_END() outb(0x20, 0x20)
#define INT_END_SLAVE() outb(0xA0, 0x20); outb(0x20, 0x20)

// Handlers are called when there is an interrupt
u32 irq0Handler()
{
    INT_END();

    return 0;
}

// Keyboard
u32 irq1Handler()
{
    // Dispatch event
    onKeyPressed();

    INT_END();

    return 0;
}

u32 irq2Handler()
{
    INT_END();

    return 0;
}

u32 irq3Handler()
{
    INT_END();

    return 0;
}

u32 irq4Handler()
{
    INT_END();

    return 0;
}

u32 irq5Handler()
{
    INT_END();

    return 0;
}

u32 irq6Handler()
{
    INT_END();

    return 0;
}

u32 irq7Handler()
{
    INT_END();

    return 0;
}

u32 irq8Handler()
{
    INT_END_SLAVE();

    return 0;
}

u32 irq9Handler()
{
    INT_END_SLAVE();

    return 0;
}

u32 irq10Handler()
{
    INT_END_SLAVE();

    return 0;
}

u32 irq11Handler()
{
    INT_END_SLAVE();

    return 0;
}

u32 irq12Handler()
{
    INT_END_SLAVE();

    return 0;
}

u32 irq13Handler()
{
    INT_END_SLAVE();

    return 0;
}

u32 irq14Handler()
{
    INT_END_SLAVE();

    return 0;
}

u32 irq15Handler()
{
    INT_END_SLAVE();

    return 0;
}
