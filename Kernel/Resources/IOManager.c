// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <IOManager.h>
#include <taskManager.h>
#include <stringLib.h>
#include <keyboardDriver.h>
#include <pipes.h>

void fprintfBR(char* string, uint8_t lenght, t_colour bgColour, t_colour fontColour) {
      uint16_t outfd = getCurrentProcessOutFd();

      if (outfd == 0)
            sys_write(string, lenght, bgColour, fontColour);
      else
            writePipe(outfd, string);
      
}

int getchar() {
      uint16_t infd = getCurrentProcessInFd();

      if (infd == 0) {
            if (currentProcessFg())
                  return getKeyboardChar();
            else
                  return -1;
      }

      return readPipe(infd);
}