#include "color.h"

#include "drivers/screen.h"
#include "drivers/console.h"
#include <stdio.h>
#include <string.h>

// All colors by name
static const char *colNames[] = {
    "red",
    "blue",
    "green",
    "yellow",
    "gray",
};

// Formats (default)
static const uint8_t colFmt[] = {
    FMT_RED,
    FMT_BLUE,
    FMT_GREEN,
    FMT_YELLOW,
    FMT_GRAY,
};

// Searches a command
// - Returns len(colNames) if not found
static size_t findColor(const char *name)
{
    // Number of colors
    size_t n = sizeof(colNames) / sizeof(const char*);

    // Find color
    for (size_t i = 0; i < n; ++i)
        if (strcmp(name, colNames[i]) == 0)
            return i;

    // Not found
    return n;
}

int colorMain(int argc, char **argv)
{
    size_t nColors = sizeof(colNames) / sizeof(const char*);

    // TODO : To lower

    if (argc == 2)
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            puts("Usage :");
            puts("- color list : Lists all colors");
            puts("- color reset : Sets the format to default");
            puts("- color <color> : Sets foreground color");
            puts("- color <foreground> <background> : Sets foreground and background color");
        }
        else if (strcmp(argv[1], "reset") == 0)
            consoleFmt = FMT_DEFAULT;
        else if (strcmp(argv[1], "list") == 0)
            for (size_t i = 0; i < nColors; ++i)
                puts(colNames[i]);
        else
        {
            // Foreground
            size_t colId = findColor(argv[1]);

            if (colId == nColors)
            {
                // TODO : stderr
                printf("Color <%s> not found, use color list to show available colors\n", argv[0]);
                return -1;
            }
            
            uint8_t color = colFmt[colId];

            // Set foreground color
            consoleFmt = (consoleFmt & 0b11110000) | FMT_TO_LIGHT(color);
        }
    }
    else if (argc == 3)
    {
        // Foreground
        size_t colId = findColor(argv[1]);

        if (colId == nColors)
        {
            // TODO : stderr
            printf("Color <%s> not found, use color list to show available colors\n", argv[0]);
            return -1;
        }
        
        uint8_t fg = FMT_TO_LIGHT(colFmt[colId]);

        // Background
        colId = findColor(argv[2]);

        if (colId == nColors)
        {
            // TODO : stderr
            printf("Color <%s> not found, use color list to show available colors\n", argv[1]);
            return -1;
        }
        
        uint8_t bg = colFmt[colId];

        // TODO : LIGHT

        // Set format
        consoleFmt = (bg << 4) | fg;

    }
    else
    {
        // TODO : stderr printf
        printf("Invalid arguments, use color --help to see the usage\n");   
        return -1;
    }

    return 0;
}
