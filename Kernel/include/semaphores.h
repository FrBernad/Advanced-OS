#ifndef SEMAPHORES_H
#define SEMAPHORES_H

int sem_open(char* name, uint64_t initialCount);

int sem_wait(int semIndex);

int sem_post(int semIndex);

int sem_close(int semIndex);

#endif