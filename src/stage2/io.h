#pragma once

// IO functions

#define flush() fill(0)

// --- Terminal --- //
// Fills the screen with blank pixels
// error : If 1, fills screen with blue
void fill(int error);

// Prints with CRLF at the end
void puts(const char *s);

// Fatal error
void fatal(const char *s);

void newLine();
void putchar(char c);

// Updates the display of the cursor
void updateCursor();

// Freezes the screen
void halt();

// --- HDD --- //
// Specs for ATA disks IO : https://wiki.osdev.org/ATA_PIO_Mode
// Identifies the disk and verify that the disk is ATA
void identifyDisk();

// Reads the ATA disk (LBA28)
void readDisk(int sector, void *dst);
