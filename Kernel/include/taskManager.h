#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#define MAX_PROCESSES 2

#include <stdint.h>
#include <screens.h>

void initScheduler();
void* scheduler(void* oldRSP);
uint64_t addProcess(void (*entryPoint)(int, char**), int argc, char** argv, uint8_t fg, uint16_t * fd);
void listProcesses();
uint64_t killProcess(uint64_t pid);
uint64_t changePriority(uint64_t pid, uint64_t priority);
uint64_t blockProcess(uint64_t pid);
uint64_t unblockProcess(uint64_t pid);
void resignCPU();
void yield();
uint64_t currentProcessPid();
void killForeground();
int getCurrentProcessInFd();
int getCurrentProcessOutFd();
int currentProcessFg();
void wait(uint64_t pid);

#endif