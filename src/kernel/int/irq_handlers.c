#include "irq_handlers.h"

#include "drivers/ports.h"
#include "drivers/console.h"

#define INT_END() outb(0x20, 0x20)
#define INT_END_SLAVE() outb(0xA0, 0x20); outb(0x20, 0x20)

// Handlers are called when there is an interrupt
void irq0Handler()
{
    // Send acknowledgement
    INT_END();
}

// Keyboard
void irq1Handler()
{
    // Dispatch
    // TODO :
    consolePut(inb(0x60));

    // Send acknowledgement
    INT_END();
}

void irq2Handler()
{
    // Send acknowledgement
    INT_END();
}

void irq3Handler()
{
    // Send acknowledgement
    INT_END();
}

void irq4Handler()
{
    // Send acknowledgement
    INT_END();
}

void irq5Handler()
{
    // Send acknowledgement
    INT_END();
}

void irq6Handler()
{
    // Send acknowledgement
    INT_END();
}

void irq7Handler()
{
    // Send acknowledgement
    INT_END();
}

void irq8Handler()
{
    // Send acknowledgement
    INT_END_SLAVE();
}

void irq9Handler()
{
    // Send acknowledgement
    INT_END_SLAVE();
}

void irq10Handler()
{
    // Send acknowledgement
    INT_END_SLAVE();
}

void irq11Handler()
{
    // Send acknowledgement
    INT_END_SLAVE();
}

void irq12Handler()
{
    // Send acknowledgement
    INT_END_SLAVE();
}

void irq13Handler()
{
    // Send acknowledgement
    INT_END_SLAVE();
}

void irq14Handler()
{
    // Send acknowledgement
    INT_END_SLAVE();
}

void irq15Handler()
{
    // Send acknowledgement
    INT_END_SLAVE();
}
