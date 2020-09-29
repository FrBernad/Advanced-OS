#ifndef UTILS_H
#define UTILS_H

#include <buffer.h>
#include <stdint.h>

#define IS_LETTER(c) (c >= 'a' && c <= 'z' ? 1 : 0)
#define IS_OPERAND(c) (c == '+' || c == '-' || c == '*' || c == '%' || c == '(' || c == ')' ? 1 : 0)
#define IS_DIGIT(c) (c >= '0' && c <= '9' ? 1 : 0)
#define ABS(c) (c >= 0 ? c : c * -1)
#define MAX(a, b) (a > b ? a : b)

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
uint32_t uintToBaseWL(uint64_t value, char *buffer, uint32_t base, uint32_t lenght);
uint8_t BSDToInt(uint8_t num);
uint64_t strToInt(char *str, int *error);
uint64_t strToHex(char *str, int *error);
uint64_t pow(uint64_t x, uint64_t y);
char *strtok(char *string, char *result, const char delim);
uint8_t stringcmp(char *str1, char *str2);
int isNum(char *str);
int ticksElapsed();
void cleanBuffer(t_buffer *buffer);
void cleanString(char *str);
void *memcpy(void *destination, const void *source, uint64_t length);
void strToDouble(char *numStr, int *error, double *result);
void doubleToString(char *res, double total, int afterpoint);
void reverse2(char *str, int len);
int intToStr(int x, char str[], int d);
char *itoa(int value, char *buffer, int base);
void sleep(int segs);

#endif