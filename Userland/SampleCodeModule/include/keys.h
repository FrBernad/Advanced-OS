#ifndef KEYS_H
#define KEYS_H

#define KEYS 59

typedef enum {
      PRESSED = 1,
      RELEASED = 2,
      ERROR = -1
} t_keyState;

typedef enum {
      CLEAR_SCREEN = 0x16,
} t_specialKeyCode;

#endif