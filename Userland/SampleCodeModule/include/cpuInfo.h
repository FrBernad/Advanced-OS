#ifndef CPU_INFO_H
#define CPU_INFO_H

#include <stdint.h>

typedef struct {
      char * cpuVendor;
      uint8_t model;
} t_cpuInfo;

#endif
