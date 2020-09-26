#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

#define BUFFER_SIZE 150

typedef struct {
      char buffer[BUFFER_SIZE];
      uint8_t index;
} t_buffer;

#endif