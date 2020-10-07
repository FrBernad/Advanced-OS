#ifndef SYSTEM_CALLS
#define SYSTEM_CALLS

//dataTypes
#include <RTCTime.h>
#include <taskManager.h>
#include <colours.h>
#include <cpuInfo.h>
#include <stdint.h>
#include <RTCTime.h>

void sys_getMem(uint64_t memDir, uint8_t* memData);
uint8_t sys_RTCTime(t_timeInfo info);
int sys_temp();
void sys_write(char* string, uint8_t lenght, t_colour bgColour, t_colour fontColour);
char sys_getchar();
void sys_clear();
uint64_t sys_loadApp(void (*entryPoint)(int, char**), int argc, char** argv);
uint64_t * sys_inforeg();
void sys_ps();
int sys_secsElapsed();
uint64_t sys_kill(uint64_t pid);
uint64_t sys_nice(uint64_t pid, uint64_t priority);
uint64_t sys_block(uint64_t pid);
uint64_t sys_getPID();
void *sys_malloc(uint32_t nbytes);
void sys_free(void* ptr);
void sys_yield();

#endif