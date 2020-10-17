#ifndef SCREENS_H
#define SCREENS_H

#include <stdint.h>

typedef struct
{
    int defaultBGColour;
    int defaultFontColour;
    uint32_t currentX;
    uint32_t currentY;
    uint32_t margin;
    uint32_t width;
    uint32_t height;
} t_screen;

#endif
