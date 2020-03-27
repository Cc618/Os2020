#pragma once

#include <stdint.h>

// Format
extern uint8_t consoleFmt;

// Put a new char in the screen
void consolePut(char c);

// consolePut('\n')
void consoleNewLine();
