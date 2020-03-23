#pragma once

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// TODO : Format enum

// Sets the char c at pos (x, y) with format fmt
// - Returns non zero if the position is invalid
int setChar(unsigned int x, unsigned int y, char c, char fmt);

// Fills the screen
void fillScreen(char c, char fmt);

// Sets the caret position
// - Returns non zero if the position is invalid
int setCaret(unsigned int x, unsigned int y);
