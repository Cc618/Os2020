#include "irq_handlers.h"

#include "drivers/ports.h"
#include "drivers/keyboard.h"
#include "syscalls/syscalls.h"

#define INT_END() outb(0x20, 0x20)
#define INT_END_SLAVE() outb(0xA0, 0x20); outb(0x20, 0x20)

// Handlers are called when there is an interrupt
void irq0Handler()
{
    INT_END();
}

// Keyboard
void irq1Handler()
{
    INT_END();

    // Dispatch event
    onKeyPressed();
}

void irq2Handler()
{
    INT_END();
}

void irq3Handler()
{
    INT_END();
}

void irq4Handler()
{
    INT_END();
}

void irq5Handler()
{
    INT_END();
}

void irq6Handler()
{
    INT_END();
}

void irq7Handler()
{
    INT_END();
}

void irq8Handler()
{
    INT_END_SLAVE();
}

void irq9Handler()
{
    INT_END_SLAVE();
}

void irq10Handler()
{
    INT_END_SLAVE();
}

void irq11Handler()
{
    INT_END_SLAVE();
}

void irq12Handler()
{
    INT_END_SLAVE();
}

void irq13Handler()
{
    INT_END_SLAVE();
}

void irq14Handler()
{
    INT_END_SLAVE();
}

void irq15Handler()
{
    INT_END_SLAVE();
}
