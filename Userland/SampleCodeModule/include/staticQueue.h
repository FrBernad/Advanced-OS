#ifndef STATIC_QUEUE
#define STATIC_QUEUE

#include <stdint.h>

#define MAX_SIZE 50

typedef struct {
      void* queue;
      int front;
      int rear;
      int size;
      int dim;
      uint64_t dataSize;
} t_queue;

void queuePeek(t_queue * queue,void * data);
int queueIsEmpty(t_queue* queue);
int queueIsFull(t_queue* queue);
int queueSize(t_queue* queue);
void queueInsert(t_queue* queue, void* data);
void queueRemoveData(t_queue* queue, void* data);

#endif