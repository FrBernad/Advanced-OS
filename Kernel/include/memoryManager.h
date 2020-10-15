#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>

void initMemoryManager(void *memBase, uint64_t memSize);
void *mallocBR(uint32_t nbytes);
void freeBR(void *ptr);
void dumpMM();

#endif
