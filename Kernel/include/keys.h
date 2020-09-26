#ifndef KEYS_H
#define KEYS_H

#define KEYS 59

typedef enum {
      PRESSED = 1,
      RELEASED = 2,
      ERROR = -1
} t_keyState;

typedef enum {
      L_SHFT_SC = 0x2A,
      R_SHFT_SC = 0x36,
      CAPS_LCK_SC = 0x3A,
      L_CONTROL_SC = 0x1D,
} t_specialKeySC;

typedef enum {
      CLEAR_SCREEN = 0x16,
} t_specialKeyCode;

#endif