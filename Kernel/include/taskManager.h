#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#define MAX_PROCESSES 2

#include <stdint.h>
#include <screens.h>

void initScheduler();
void* scheduler(void* oldRSP);
int addProcess(void (*entryPoint)(int, char**), int argc, char** argv);
void killCurrentProcess();
void resetCurrentProcess();

#endif