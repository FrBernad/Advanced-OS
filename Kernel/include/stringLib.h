#ifndef STRING_LIB_H
#define STRING_LIB_H

#include <stdint.h>
#include <colours.h>

void sys_write(char* string, uint8_t lenght, t_colour bgColour, t_colour fontColour);
void sys_staticwrite(char* string, uint8_t lenght, t_colour bgColour, t_colour fontColour);
void printString(char* str);
void printStringLn(char* str);
void printStringWC(char* str, t_colour bgColour, t_colour fontColour);
void printInt(uint64_t num);
void printHex(uint64_t num);
void printHexWLC(uint64_t num, int lenght, t_colour bgColour, t_colour fontColour);
void printHexWC(uint64_t num, t_colour bgColour, t_colour fontColour);
void putchar(char c);
void putcharWC(char c, t_colour bgColour, t_colour fontColour);
void staticputchar(char c);
int strlen(char* str);
int printfBR(char const* fmt, ...);

#endif