#include "drivers/ports.h"
// #include "drivers/keyboard.h"

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// Loads the IDT, defined in kernel.asm
extern void loadIDT();

// Interrupts defined in kernel/lowlevel.asm
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

typedef struct IDTEntry_t
{
	uint16_t offsetLow;
	uint16_t selector;
	uint8_t zero;
	uint8_t flags;
	uint16_t offsetHigh;
} __attribute__((packed)) IDTEntry;

// The IDT (256 entries)
IDTEntry IDT[256];

// Global variable which stores a pointer to the IDT descriptor
uint32_t idtDescriptor[2];

// Inspired by https://arjunsreedharan.org/post/99370248137/kernels-201-lets-write-a-kernel-with-keyboard
void remapPIC()
{
	//      	Ports :
	//				PIC1	PIC2
	// 	Command 	0x20	0xA0
	// 	Data	 	0x21	0xA1

	// ICW1 : Begin initialization
	outb(0x11, 0x20);
	outb(0x11, 0xA0);

	// ICW2 : Remap offset address of IDT
	outb(0x20, 0x21);
	outb(0x28, 0xA1);

	// ICW3 : Setup cascading
	outb(0x04, 0x21);
	outb(0x02, 0xA1);

	// ICW4 : Environment info
	outb(0x01, 0x21);
	outb(0x01, 0xA1);

	// Mask interrupts
	outb(0x0, 0x21);
	outb(0x0, 0xA1);
}

void initInterruptEntry(IDTEntry *entry, const uint32_t IRQ_ADDRESS)
{
#define CODE_SEGMENT_OFFSET 8

	entry->offsetLow = IRQ_ADDRESS & 0xFFFF;
	entry->selector = CODE_SEGMENT_OFFSET;
	entry->zero = 0;
	// Interrupt gate
	entry->flags = 0x8E;
	entry->offsetHigh = (IRQ_ADDRESS & 0xFFFF0000) >> 16;
}

void initInterrupts()
{
	// Setup PIC
	remapPIC();

	// Setup IRQs
	void (*IRQ_TABLE[16])() = {
		irq0,
		irq1,
		irq2,
		irq3,
		irq4,
		irq5,
		irq6,
		irq7,
		irq8,
		irq9,
		irq10,
		irq11,
		irq12,
		irq13,
		irq14,
		irq15
	};

	for (size_t i = 32; i < 48; ++i)
		initInterruptEntry(&IDT[i], (uint32_t)IRQ_TABLE[i - 32]);

	// Setup descriptor
	const uint32_t IDT_ADDRESS = (uint32_t)IDT;
	idtDescriptor[0] = (sizeof(IDTEntry) * 256) + ((IDT_ADDRESS & 0xFFFF) << 16);
	idtDescriptor[1] = IDT_ADDRESS >> 16;

	// Load the IDT with the descriptor
	loadIDT();
}

// To tell to the PIC that is the end of the interrupt
void endOfInterrupt()
{
	outb(0x20, 0x20);
}

void endOfInterruptLong()
{
	outb(0xA0, 0x20);
	outb(0x20, 0x20);
}

// Handlers //
// Handlers are called when there is an interrupt
void irq0Handler()
{
    // Send acknowledgement
    endOfInterrupt();
}

// Keyboard
void irq1Handler()
{
    // Dispatch
    // onKeyInterrupt();
    putchar(inb(0x60));

    // Send acknowledgement
    endOfInterrupt();
}

void irq2Handler()
{
    // Send acknowledgement
    endOfInterrupt();
}

void irq3Handler()
{
    // Send acknowledgement
    endOfInterrupt();
}

void irq4Handler()
{
    // Send acknowledgement
    endOfInterrupt();
}

void irq5Handler()
{
    // Send acknowledgement
    endOfInterrupt();
}

void irq6Handler()
{
    // Send acknowledgement
    endOfInterrupt();
}

void irq7Handler()
{
    // Send acknowledgement
    endOfInterrupt();
}

void irq8Handler()
{
    // Send acknowledgement
    endOfInterruptLong();
}

void irq9Handler()
{
    // Send acknowledgement
    endOfInterruptLong();
}

void irq10Handler()
{
    // Send acknowledgement
    endOfInterruptLong();
}

void irq11Handler()
{
    // Send acknowledgement
    endOfInterruptLong();
}

void irq12Handler()
{
    // Send acknowledgement
    endOfInterruptLong();
}

void irq13Handler()
{
    // Send acknowledgement
    endOfInterruptLong();
}

void irq14Handler()
{
    // Send acknowledgement
    endOfInterruptLong();
}

void irq15Handler()
{
    // Send acknowledgement
    endOfInterruptLong();
}
