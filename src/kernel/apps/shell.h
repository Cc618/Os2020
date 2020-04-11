#pragma once

#include <stdbool.h>

int shellMain(int argc, char **argv);

// Displays the PS1
void shellPS1();

// When backspace is pressed
bool shellDelete();

// Evaluates a string as a command
void shellEval(const char *cmd);
