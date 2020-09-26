#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

#define REGISTERS 17
#define REG_NAME 10

typedef struct {
      uint64_t * data;
      char name[REGISTERS][REG_NAME];
} t_registers;

#endif
