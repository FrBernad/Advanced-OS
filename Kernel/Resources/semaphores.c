#include <lib.h>
#include <memoryManager.h>
#include <semaphores.h>
#include <stddef.h>
#include <stdint.h>
#include <taskManager.h>
#include <utils.h>

#define MAX_SEMAPHORES 6

typedef struct pNode {
      uint64_t processPID;
      struct pNode* next;
} t_pNode;

typedef struct pList {
      uint32_t size;
      t_pNode* first;
      t_pNode* last;
} t_pList;

typedef struct {
      uint16_t attachedProcesses;
      t_pList* blockedProcesses;
      char* name;
      uint8_t active;
      uint16_t count;
      uint8_t lock;
} t_sem;

static int createSem(char* name, uint64_t initialCount);
static int searchSemaphore(char* name);
static int getAvailableSem();

static int queueIsEmpty(t_pList* blockedList);
static void enqueueProcess(t_pList* blockedList, t_pNode* newProcess);
static uint64_t dequeueProcess(t_pList* blockedList);
static void freeList(t_pList* list);

static t_sem semaphores[MAX_SEMAPHORES];
static uint32_t activeSemaphores;
static uint8_t globalSemLock;

int sem_open(char* name, uint64_t initialCount) {
      if (name == NULL)
            return -1;

      int semIndex;

      lock_region(&globalSemLock);

      if (activeSemaphores > 0)
            semIndex = searchSemaphore(name);

      if (semIndex == -1) {
            semIndex = createSem(name, initialCount);
            if (semIndex == -1)
                  return -1;
      }

      semaphores[semIndex].attachedProcesses++;

      unlock_region(&globalSemLock);

      return semIndex;
}

int sem_wait(int semIndex) {
      t_sem sem = semaphores[semIndex];

      if (!sem.active)
            return -1;

      lock_region(&sem.lock);

      if (sem.count > 0) {
            sem.count--;
            unlock_region(&sem.lock);
            return 1;
      }

      uint64_t pid = currentProcessPid();
      t_pNode* p = mallocBR(sizeof(t_pNode));
      if (p == NULL) {
            unlock_region(&sem.lock);
            return -1;
      }
      p->processPID = pid;
      enqueueProcess(sem.blockedProcesses, p);

      unlock_region(&sem.lock);

      blockProcess(pid);

      return 1;
}

int sem_post(int semIndex) {
      t_sem sem = semaphores[semIndex];

      if (!sem.active)
            return -1;

      lock_region(&sem.lock);

      if (sem.count > 0) {
            sem.count++;
            unlock_region(&sem.lock);
            return 1;
      }
      uint64_t processPID = dequeueProcess(sem.blockedProcesses);

      unlock_region(&sem.lock);

      blockProcess(processPID);

      return 1;
}

int sem_close(int semIndex) {
      t_sem sem = semaphores[semIndex];
      if (!sem.active)
            return -1;

      lock_region(&sem.lock);

      sem.attachedProcesses--;

      if (sem.attachedProcesses > 0) {
            unlock_region(&sem.lock);
            return 1;
      }

      if (sem.blockedProcesses->size > 0) {
            printfBR("Error closing semaphore, processes still blocked\n");
            unlock_region(&sem.lock);
            return -1;
      }

      sem.active = 0;
      freeList(sem.blockedProcesses);

      unlock_region(&sem.lock);

      return 1;
}

static int createSem(char* name, uint64_t initialCount) {
      int semIndex = getAvailableSem();
      if (semIndex == -1) {
            return -1;
      }
      t_sem sem = semaphores[semIndex];
      sem.name = name;
      sem.active = 1;
      sem.count = initialCount;
      sem.blockedProcesses = mallocBR(sizeof(t_pList));
      if (sem.blockedProcesses == NULL)
            return -1;
      sem.blockedProcesses->first = NULL;
      sem.blockedProcesses->last = sem.blockedProcesses->first;
      sem.blockedProcesses->size = 0;
      return semIndex;
}

static int getAvailableSem() {
      for (size_t i = 0; i < MAX_SEMAPHORES; i++) {
            if (!semaphores[i].active) {
                  return i;
            }
      }
      return -1;
}

static int searchSemaphore(char* name) {
      for (int i = 0; i < MAX_SEMAPHORES; i++) {
            if (semaphores[i].active) {
                  if (stringcmp(name, semaphores[i].name) == 0)
                        return i;
            }
      }
      return -1;
}

static int queueIsEmpty(t_pList* blockedList) {
      return blockedList->size == 0;
}

static void enqueueProcess(t_pList* blockedList, t_pNode* newProcess) {
      if (queueIsEmpty(blockedList)) {
            blockedList->first = newProcess;
            blockedList->last = blockedList->first;
      } else {
            blockedList->last->next = newProcess;
            newProcess->next = NULL;
            blockedList->last = newProcess;
      }
      blockedList->size++;
}

static uint64_t dequeueProcess(t_pList* blockedList) {
      if (queueIsEmpty(blockedList))
            return 0;

      t_pNode* p = blockedList->first;
      blockedList->first = blockedList->first->next;
      blockedList->size--;
      uint64_t processPID=p->processPID;
      freeBR(p);
      return processPID;
}

static void freeList(t_pList* list) {
      t_pNode *p = list->first, *aux;

      while (p != NULL) {
            aux = p;
            p = p->next;
            freeBR(aux);
      }
      freeBR(list);
}
