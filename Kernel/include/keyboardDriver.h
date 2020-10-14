#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <screens.h>
#include <stdint.h>

void keyboardHandler(uint64_t * rsp);
int getKeyboardChar();
uint64_t* getSnapshot();
int initKeyboardHandler();

#endif