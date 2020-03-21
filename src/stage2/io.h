#pragma once

// IO functions

// --- Terminal --- //
// Fills the screen with black pixels
void flush();

// Prints with CRLF at the end
void puts(const char *s);

void newLine();
void putchar(char c);

// Updates the display of the cursor
void updateCursor();

// --- HDD --- //
