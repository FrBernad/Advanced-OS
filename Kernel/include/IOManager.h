#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <colours.h>
#include <stdint.h>

void fprintfBR(char* string, uint8_t lenght, t_colour bgColour, t_colour fontColour);
int getchar();

#endif