// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdarg.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <stddef.h>
#include <utils.h>

static int processString(char const* fmt, va_list arg);

void printString(char* str) {
      sys_write(str, strlen(str), BLACK, WHITE);
}

void printStringWC(char* str, t_colour bgColour, t_colour fontColour) {
      sys_write(str, strlen(str), bgColour, fontColour);
}

void printStringLn(char* str) {
      printString(str);
      putchar('\n');
}

int getchar(){
      return sys_getchar();
}

void putchar(char c) {
      sys_write(&c, 1, BLACK, WHITE);
}

void putcharWC(char c, t_colour bgColour, t_colour fontColour) {
      sys_write(&c, 1, bgColour, fontColour);
}

void printHex(uint64_t num) {
      char buffer[10];
      uintToBase(num, buffer, 16);
      printString(buffer);
}

void printHexWL(uint64_t num, int lenght) {
      char buffer[20];
      uintToBaseWL(num, buffer, 16,8);
      printString(buffer);
}

void printInt(uint64_t num) {
      char buffer[10];
      uintToBase(num, buffer, 10);
      printString(buffer);
}

void clear() {
      sys_clear();
}

void deletechar() {
      putchar('\b');
}

char* strcpy(char* destination, char* source) {
      // return if no memory is allocated to the destination
      if (destination == NULL)
            return NULL;

      // take a pointer pointing to the beginning of destination string
      char* ptr = destination;

      // copy the C-string pointed by source into the array
      // pointed by destination
      while (*source != '\0') {
            *destination = *source;
            destination++;
            source++;
      }

      // include the terminating null character
      *destination = '\0';

      // destination is returned by standard strcpy()
      return ptr;
}

int printfBR(char const* fmt, ...) {
      va_list arg;
      int length;

      va_start(arg, fmt);
      length = processString(fmt, arg);
      va_end(arg);
      return length;
}

static int processString(char const* fmt, va_list arg) {
      int int_temp;
      char char_temp;
      char* string_temp;

      char ch;
      int length = 0;

      char buffer[512];

      while ((ch = *fmt++)) {
            if ('%' == ch) {
                  switch (ch = *fmt++) {
                        /* %% - print out a single %    */
                        case '%':
                              putchar('%');
                              length++;
                              break;

                        /* %c: print out a character    */
                        case 'c':
                              char_temp = va_arg(arg, int);
                              putchar(char_temp);
                              length++;
                              break;

                        /* %s: print out a string       */
                        case 's':
                              string_temp = va_arg(arg, char*);
                              printString(string_temp);
                              length += strlen(string_temp);
                              break;

                        /* %d: print out an int         */
                        case 'd':
                              int_temp = va_arg(arg, int);
                              itoa(int_temp, buffer, 10);
                              printString(buffer);
                              length += strlen(buffer);
                              break;

                        /* %x: print out an int in hex  */
                        case 'x':
                              int_temp = va_arg(arg, int);
                              itoa(int_temp, buffer, 16);
                              printString("0x");
                              printString(buffer);
                              length += strlen(buffer);
                              break;
                  }
            } else {
                  putchar(ch);
                  length++;
            }
      }
      return length;
}

char* strcat(char* destination, char* source) {
      // make ptr point to the end of destination string
      char* ptr = destination + strlen(destination);

      // Appends characters of source to the destination string
      while (*source != '\0')
            *ptr++ = *source++;

      // null terminate destination string
      *ptr = '\0';

      // destination is returned by standard strcat()
      return destination;
}