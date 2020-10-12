#include <pipes.h>
#include <semaphores.h>
#include <stdint.h>
#include <stringLib.h>
#include <utils.h>

#define MAX_PIPES 20
#define PIPE_BUF 1024

typedef struct {
      char buffer[PIPE_BUF];
      char* name;
      uint16_t dim;
      uint16_t front;
      uint16_t rear;
      uint16_t size;
      uint16_t attachedProcesses;

      uint16_t writeLock;
      uint16_t readLock;
      uint8_t active;
} t_pipe;

typedef struct {
      t_pipe pipes[MAX_PIPES];
      uint16_t lock;
} t_pipes;

static int createPipe(char* pipeName);
static int getAvailablePipe();
static int searchPipe(char* pipeName);
static int isValid(int pipeIndex);
static void dumpPipe(int pipeIndex);

//queue functions
static int queueIsFull(t_pipe* pipe);
static void queueInsert(t_pipe* pipe, char data);
static int queueRemoveData(t_pipe* pipe);

static t_pipes pipes;

int initPipes() {
      if ((pipes.lock = sem_open("pipes_lock", 1)) == -1) {
            return -1;
      }
      return 0;
}

int popen(char* pipeName) {
      sem_wait(pipes.lock);

      int pipeIndex = searchPipe(pipeName);
      if (pipeIndex == -1) {
            pipeIndex = createPipe(pipeName);
            if (pipeIndex == -1) {
                  sem_post(pipes.lock);
                  return -1;
            }
      }

      pipes.pipes[pipeIndex].attachedProcesses++;

      sem_post(pipes.lock);

      return pipeIndex;
}

int closePipe(int pipeIndex) {
    if(!isValid(pipeIndex))
        return -1;

    sem_wait(pipes.lock);

    t_pipe* pipe = &pipes.pipes[pipeIndex];

    pipe->attachedProcesses--;

    if (pipe->attachedProcesses > 0) {
          sem_post(pipes.lock);
          return 1;
    }

    sem_close(pipe->readLock);
    sem_close(pipe->writeLock);

    pipe->active=0;

    sem_post(pipes.lock);

    return 1;
}


int writePipe(int pipeIndex, char* string) {
      if (!isValid(pipeIndex))
            return -1;

      while (*string != 0) {
            writeCharPipe(pipeIndex, *string++);
      }

      return 0;
}

int readPipe(int pipeIndex) {
      if (!isValid(pipeIndex))
            return -1;

      t_pipe* pipe = &pipes.pipes[pipeIndex];

      sem_wait(pipe->readLock);

      int c = queueRemoveData(pipe);

      sem_post(pipe->writeLock);

      return c;
}

int writeCharPipe(int pipeIndex, char c) {
      if (!isValid(pipeIndex))
            return -1;

      t_pipe* pipe = &pipes.pipes[pipeIndex];

      sem_wait(pipe->writeLock);

      queueInsert(pipe, c);

      sem_post(pipe->readLock);

      return 0;
}

void dumpPipes(){
    for (int i = 0; i < MAX_PIPES; i++)
    {
        if(pipes.pipes[i].active){
            printfBR("Pipe index: %d\n",i);
            dumpPipe(i);
        }
    }
}

static void dumpPipe(int pipeIndex){
      t_pipe* pipe = &pipes.pipes[pipeIndex];

      printfBR("Name: %s\n", pipe->name);
      printfBR("atatchedProcesses: %s\n", pipe->attachedProcesses);
      printfBR("read sem: %d\n", pipe->readLock);
      printfBR("write sem: %d\n", pipe->writeLock);

      for (int i = pipe->front; i != pipe->rear; i = i + 1 % PIPE_BUF)
            putchar(pipe->buffer[i]);

      printfBR("\n");

      printfBR("blocked by read: \n");
      dumpSemaphore(pipe->readLock);

      printfBR("blocked by write: \n");
      dumpSemaphore(pipe->writeLock);
}

static int createPipe(char* pipeName) {
      int pipeIndex = getAvailablePipe();
      if (pipeIndex == -1) {
            return -1;
      }

      t_pipe* pipe = &pipes.pipes[pipeIndex];

      pipe->active = 1;
      pipe->name = pipeName;
      pipe->dim = PIPE_BUF;
      pipe->size = 0;
      pipe->rear = -1;
      pipe->front = 0;
      pipe->attachedProcesses = 0;

      char readSem[25] = {0};
      strcpy(readSem, pipeName);
      strcat(readSem, "_R");

      if ((pipe->readLock = sem_open(readSem, 0)) == -1) {
            return -1;
      }
      char writeSem[25] = {0};
      strcpy(writeSem, pipeName);
      strcat(writeSem, "_W");
      if ((pipe->readLock = sem_open(writeSem, PIPE_BUF)) == -1) {
            return -1;
      }

      return pipeIndex;
}

static int getAvailablePipe() {
      for (int i = 0; i < MAX_PIPES; i++) {
            if (!pipes.pipes[i].active) {
                  return i;
            }
      }
      return -1;
}

static int searchPipe(char* pipeName) {
      for (int i = 0; i < MAX_PIPES; i++) {
            if (pipes.pipes[i].active) {
                  if (stringcmp(pipeName, pipes.pipes[i].name) == 0) {
                        return i;
                  }
            }
      }
      return -1;
}

static int isValid(int pipeIndex) {
      if (pipeIndex < 0 || pipeIndex > MAX_PIPES || !pipes.pipes[pipeIndex].active)
            return 0;
      return 1;
}

static int queueIsFull(t_pipe* pipe) {
      return pipe->size == pipe->dim;
}

static void queueInsert(t_pipe* pipe, char data) {
      if (!queueIsFull(pipe)) {
            if (pipe->rear == pipe->dim - 1) {
                  pipe->rear = -1;
            }
            pipe->buffer[++pipe->rear] = data;
            pipe->size++;
      }
}

static int queueRemoveData(t_pipe* pipe) {
      int toReturn;
      if (pipe->size != 0) {
            toReturn = pipe->buffer[pipe->front++];
            if (pipe->front == pipe->dim) {
                  pipe->front = 0;
            }
            pipe->size--;
      } else {
            toReturn = -1;
      }
      return toReturn;
}
