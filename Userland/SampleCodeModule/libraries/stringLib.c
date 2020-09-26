#include <stringLib.h>
#include <systemCalls.h>
#include <utils.h>

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
