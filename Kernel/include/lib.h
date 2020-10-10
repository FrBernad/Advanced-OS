#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void* memcpy(void* destination, const void* source, uint64_t length);

char * cpuVendor(char * result);
uint64_t cpuModel();
int cpuTemp();
void sys_getMem(uint64_t memDir, uint8_t* memData);
uint8_t getBSDTimeInfo(uint8_t info);

void callTimerTick ();

void lock_region(uint8_t * lock);
void unlock_region(uint8_t * lock);

#endif