#pragma once

#include <stdbool.h>

void shellMain();

// Displays the PS1
void shellPS1();

// When backspace is pressed
bool shellDelete();

// When pressing enter
void shellValidCommand();

// Evaluates a string as a command
void shellEval(const char *cmd);
