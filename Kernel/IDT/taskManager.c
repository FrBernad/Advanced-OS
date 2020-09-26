#include <interrupts.h>
#include <keyboardDriver.h>
#include <lib.h>
#include <memoryManager.h>
#include <stddef.h>
#include <stringLib.h>
#include <taskManager.h>

#define SIZE_OF_STACK 4 * 1024

typedef struct {
      uint64_t gs;
      uint64_t fs;
      uint64_t r15;
      uint64_t r14;
      uint64_t r13;
      uint64_t r12;
      uint64_t r11;
      uint64_t r10;
      uint64_t r9;
      uint64_t r8;
      uint64_t rsi;
      uint64_t rdi;
      uint64_t rbp;
      uint64_t rdx;
      uint64_t rcx;
      uint64_t rbx;
      uint64_t rax;

      uint64_t rip;
      uint64_t cs;
      uint64_t eflags;
      uint64_t rsp;
      uint64_t ss;
      uint64_t base;
} t_stackFrame;

typedef enum { READY,
               BLOCKED,
               KILLED } t_state;

typedef struct {
      uint64_t pid;
      char* name;
      void* rsp;
      void* rbp;
      t_state state;
} t_PCB;

typedef struct t_pNode {
      t_PCB pcb;
      struct t_pNode* next;
} t_pNode;

typedef struct pList {
      uint32_t size;
      t_pNode* first;
      t_pNode* last;
} t_pList;

static void initializeStackFrame(void (*entryPoint)(int, char**), int argc, char** argv, void* rbp);
static int initProcess(t_PCB* process, char* name);
static uint64_t newPid();
static void idleFunction(int argc, char** argv);
static void wrapper(void (*entryPoint)(int, char**), int argc, char** argv);
static void exit();
static void killProcess(uint64_t pid);

static void enqueueProcess(t_pNode* newProcess);
static t_pNode* dequeueProcess();
static int queueIsEmpty();
static void removeProcess(t_pNode* process);
static void dumpProcesses();

static t_pList* processes;
static t_pNode* currentProcess;
static t_pNode* idleProcess;
static uint64_t maxPid = 1;

void initScheduler() {
      processes = mallocBR(sizeof(t_pList));
      processes->first = NULL;
      processes->last = processes->first;
      processes->size = 0;

      char* argv[] = {"System Idle Process"};
      addProcess(&idleFunction, 1, argv);
      idleProcess = dequeueProcess();
}

void* scheduler(void* oldRSP) {
      // printString("processes: ");
      // dumpProcesses();
      if (currentProcess != NULL) {
            currentProcess->pcb.rsp = oldRSP;
            if (currentProcess->pcb.state == KILLED){
                  removeProcess(currentProcess);
            }
            else
                  enqueueProcess(currentProcess);
      }
      if (processes->size > 0) {
            currentProcess = dequeueProcess();
            while (currentProcess->pcb.state != READY) {
                  if (currentProcess->pcb.state == KILLED) {
                        removeProcess(currentProcess);
                  }
                  currentProcess = dequeueProcess();
            }
      }else
            currentProcess=idleProcess;
      
      // printString("Running process with pid ");
      // printInt(currentProcess->pcb.pid);
      // printStringLn("");

      return currentProcess->pcb.rsp;
}

int addProcess(void (*entryPoint)(int, char**), int argc, char** argv) {
      if (entryPoint == NULL)
            return 0;

      t_pNode* newProcess = mallocBR(sizeof(t_pNode));

      if (newProcess == 0)
            return 0;

      if (initProcess(&newProcess->pcb, argv[0]) == 1)
            return 0;

      initializeStackFrame(entryPoint, argc, argv, newProcess->pcb.rbp);
      enqueueProcess(newProcess);

      // printString("Adding process ");
      // printString(argv[0]);
      // printString(" with pid ");
      // printInt(newProcess->pcb.pid);
      // printStringLn("");

      return 1;
}

void resetCurrentProcess() {
      // if (queueIsEmpty(&taskManager)) {
      //       return;
      // }
      // t_PCB currentProcess;
      // queuePeek(&taskManager,&currentProcess);
      // initializeStackFrame(currentProcess.entryPoint, argc, argv, currentProcess.rbp);
      // queueUpdateFirst(&taskManager,&currentProcess);
      // sys_forceStart();
}

static int initProcess(t_PCB* process, char* name) {
      process->name = name;
      process->pid = newPid();
      process->rbp = mallocBR(SIZE_OF_STACK);
      if (process->rbp == NULL)
            return 1;
      process->rbp = (void*)((char*)process->rbp + SIZE_OF_STACK - 1);
      process->rsp = (void*)((t_stackFrame*)process->rbp - 1);
      process->state = READY;
      return 0;
}

static void wrapper(void (*entryPoint)(int, char**), int argc, char** argv) {
      entryPoint(argc, argv);

      // printString("Ending process ");
      // printString(argv[0]);
      // printString("with pid ");
      // printInt(currentProcess->pcb.pid);
      // printStringLn("");

      exit();
}

static void initializeStackFrame(void (*entryPoint)(int, char**), int argc, char** argv, void* rbp) {
      t_stackFrame* frame = (t_stackFrame*)rbp - 1;
      frame->gs = 0x001;
      frame->fs = 0x002;
      frame->r15 = 0x003;
      frame->r14 = 0x004;
      frame->r13 = 0x005;
      frame->r12 = 0x006;
      frame->r11 = 0x007;
      frame->r10 = 0x008;
      frame->r9 = 0x009;
      frame->r8 = 0x00A;
      frame->rsi = (uint64_t)argc;
      frame->rdi = (uint64_t)entryPoint;
      frame->rbp = 0x00D;
      frame->rdx = (uint64_t)argv;
      frame->rcx = 0x00F;
      frame->rbx = 0x010;
      frame->rax = 0x011;
      frame->rip = (uint64_t)wrapper;
      frame->cs = 0x008;
      frame->eflags = 0x202;
      frame->rsp = (uint64_t)(&frame->base);
      frame->ss = 0x000;
      frame->base = 0x000;
}

static uint64_t newPid() {
      return maxPid++;
}

static int queueIsEmpty() {
      return processes->size == 0;
}

static void enqueueProcess(t_pNode* newProcess) {
      if (queueIsEmpty()) {
            processes->first = newProcess;
            processes->last = processes->first;
      } else {
            processes->last->next = newProcess;
            newProcess->next = NULL;
            processes->last = newProcess;
      }
      processes->size++;
}

static void dumpProcesses() {
      if(processes->size == 0){
            printStringLn("Empty");
            return;
      }
      for (t_pNode* p = processes->first; p != NULL; p = p->next) {
            printString("process ");
            printInt(p->pcb.pid);
            printString(" ");
      }
      printStringLn("");
}

static t_pNode* dequeueProcess() {
      if (queueIsEmpty())
            return NULL;

      t_pNode* p = processes->first;
      processes->first = processes->first->next;
      processes->size--;
      return p;
}

static void removeProcess(t_pNode* process) {
      freeBR(process->pcb.rbp - SIZE_OF_STACK + 1);
      freeBR(process);
}

static void idleFunction(int argc, char** argv) {
      while (1)
            _hlt();
}

static void exit() {
      killProcess(currentProcess->pcb.pid);
      callTimerTick();
}

static void killProcess(uint64_t pid) {
      if(currentProcess->pcb.pid==pid){
            currentProcess->pcb.state = KILLED;
            return;
      }
      for (t_pNode* p = processes->first; p != NULL; p = p->next) {
            if (p->pcb.pid == pid)
                  p->pcb.state = KILLED;
      }
}