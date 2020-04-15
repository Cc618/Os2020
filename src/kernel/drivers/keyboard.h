#pragma once

#include <stdbool.h>

extern bool shiftPressed;
extern bool ctrlPressed;

void keyboardInit();
void keyboardTerminate();

// When a key is pressed on the keyboard
void onKeyPressed();
