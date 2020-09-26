#include <stdarg.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <utils.h>

static int processString(char const* fmt, va_list arg);

void printString(char* str) {
      syscall(WRITE, (uint64_t)str, strlen(str), BLACK, WHITE, 0,0);
}

void printStringWC(char* str, t_colour bgColour, t_colour fontColour) {
      syscall(WRITE, (uint64_t)str, strlen(str), bgColour, fontColour, 0, 0);
}

void printStringLn(char* str) {
      printString(str);
      putchar('\n');
}

char getchar(){
      return syscall(GETCHAR, 0, 0, 0, 0, 0, 0);
}

void putchar(char c) {
      syscall(WRITE, (uint64_t)&c, 1, BLACK, WHITE, 0, 0);
}

void putcharWC(char c, t_colour bgColour, t_colour fontColour) {
      syscall(WRITE, (uint64_t)&c, 1, bgColour, fontColour, 0, 0);
}

void printHex(uint64_t num) {
      char buffer[10];
      uintToBase(num, buffer, 16);
      printString(buffer);
}

void printHexWL(uint64_t num, int lenght) {
      char buffer[10];
      uintToBaseWL(num, buffer, 16,8);
      printString(buffer);
}

void printInt(uint64_t num) {
      char buffer[10];
      uintToBase(num, buffer, 10);
      printString(buffer);
}

void clear() {
      syscall(CLEAR, 0, 0, 0, 0, 0, 0);
}

void deletechar() {
      putchar('\b');
}

void strcpy(char* str1, char* str2){  //copies str1 into str2
      int i;
      for (i = 0; str1[i] != 0; i++) {
            str2[i]=str1[i];
      }
      str2[i]=0;
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