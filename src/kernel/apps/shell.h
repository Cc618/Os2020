#pragma once

#include <stdbool.h>

void shellInit();

// Displays the PS1
void shellPS1();

// When backspace is pressed
bool shellDelete();

// When pressing enter
void shellValidCommand();
