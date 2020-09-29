#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#define MAX_PROCESSES 2

#include <stdint.h>
#include <screens.h>

void initScheduler();
void* scheduler(void* oldRSP);
int addProcess(void (*entryPoint)(int, char**), int argc, char** argv);
void listProcesses();
void killProcess(uint64_t pid);
void changePriority(uint64_t pid, uint64_t priority);
void blockProcess(uint64_t pid);
void resignCPU();
uint64_t currentProcessPid();

#endif