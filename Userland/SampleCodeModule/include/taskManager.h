#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

typedef struct {
      void* entryPoint;
      void* rsp;
      void* rbp;
      int stackID;
} t_PCB;

void initApps();

#endif