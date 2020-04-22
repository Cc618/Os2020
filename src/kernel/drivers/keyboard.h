#pragma once

// Uses a line buffering to communicate with stdin

#include <stdbool.h>

// Max line length before the line is split (send in multiple times to stdin)
#define KBD_BUFFER_SIZE 512

extern bool shiftPressed;
extern bool ctrlPressed;

void keyboardInit();
void keyboardTerminate();

// When a key is pressed on the keyboard
void onKeyPressed();
