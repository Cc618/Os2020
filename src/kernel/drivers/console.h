#pragma once

#include <stdint.h>

// Format
extern uint8_t consoleFmt;

// Put a new char on screen
void consolePut(char c);

// Removes the last char on screen
// Does nothing if this is the start of a line
void consoleDel();

// consolePut('\n')
void consoleNewLine();

// Scrolls one line
void consoleScroll();
