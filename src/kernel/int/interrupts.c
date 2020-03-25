#include "drivers/ports.h"

#include <stdint.h>
#include <stddef.h>

// TODO : In drivers ?
// PIC ports
#define PIC_MASTER      0x20
#define PIC_MASTER_CMD  0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE       0xA0
#define PIC_SLAVE_CMD   0xA0
#define PIC_SLAVE_DATA  0xA1

// Data for PIC
#define ICW1_ICW4           0x01
#define ICW1_INIT           0x10
#define PIC_MASTER_OFFSET   0x20
#define PIC_SLAVE_OFFSET    0x28

// Io wait, can be fragile
#define PIC_WAIT() \
	do { \
		__asm__ volatile( \
            "jmp 1f\n\t" \
		    "1:\n\t" \
		    "jmp 2f\n\t" \
		    "2:"); \
	} while (0)

// Loads the IDT, defined in int/idt.asm
extern void loadIDT();

// Interrupts defined in int/idt.asm
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

// An entry in the IDT
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

// All IRQs
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

// Sets up the PIC
void initPIC()
{
	// ICW1 : Begin initialization
	outb(ICW1_ICW4 | ICW1_INIT, PIC_MASTER_CMD);
	outb(ICW1_ICW4 | ICW1_INIT, PIC_SLAVE_CMD);

    PIC_WAIT();

	// ICW2 : Remap offset address of IDT
	outb(PIC_MASTER_OFFSET, PIC_MASTER_DATA);
	outb(PIC_SLAVE_OFFSET, PIC_SLAVE_DATA);

    PIC_WAIT();

	// ICW3 : Setup cascading
	outb(0x04, PIC_MASTER_DATA);
	outb(0x02, PIC_SLAVE_DATA);

    PIC_WAIT();

	// ICW4 : Environment info
	outb(0x01, PIC_MASTER_DATA);
	outb(0x01, PIC_SLAVE_DATA);

    PIC_WAIT();

	// No masked interrupt
	outb(0, PIC_MASTER_DATA);
	outb(0, PIC_SLAVE_DATA);
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
    // Map PIC
    initPIC();

	// Init IRQs
	const size_t IRQ_START = 32;
	for (size_t i = IRQ_START; i < IRQ_START + 16; ++i)
		initInterruptEntry(&IDT[i], (uint32_t)IRQ_TABLE[i - IRQ_START]);

	// Setup descriptor
	const uint32_t IDT_ADDRESS = (uint32_t)IDT;
	idtDescriptor[0] = sizeof(IDTEntry) * 256 + ((IDT_ADDRESS & 0xFFFF) << 16);
	idtDescriptor[1] = IDT_ADDRESS >> 16;

    loadIDT();
}
