#pragma once

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define FMT_BLACK           0
#define FMT_BLUE            1
#define FMT_GREEN           2
#define FMT_CYAN            3
#define FMT_RED             4
#define FMT_PURPLE          5
#define FMT_BROWN           6
#define FMT_GRAY            7
#define FMT_DARK_GRAY       8
#define FMT_LIGHT_BLUE      9
#define FMT_LIGHT_GREEN     10
#define FMT_LIGHT_CYAN      11
#define FMT_LIGHT_RED       12
#define FMT_LIGHT_PURPLE    13
#define FMT_YELLOW          14
#define FMT_WHITE           15

// Gray on black
#define FMT_DEFAULT ((FMT_BLACK << 4) | FMT_GRAY)

// Sets the char c at pos (x, y) with format fmt
// - Returns non zero if the position is invalid
int setChar(unsigned int x, unsigned int y, char c, char fmt);

// Fills the screen
void fillScreen(char c, char fmt);

// Sets the caret position
// - Returns non zero if the position is invalid
int setCaret(unsigned int x, unsigned int y);
