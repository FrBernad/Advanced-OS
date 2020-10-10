#include <stddef.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <utils.h>

#include "test_util.h"

#define MAX_PROCESSES 10  //Should be around 80% of the the processes handled by the kernel

static uint32_t my_create_process(char* name);
static uint32_t my_kill(uint32_t pid);
static uint32_t my_block(uint32_t pid);
static uint32_t my_unblock(uint32_t pid);
static void endless_loop(int argc, char** args);

enum State { ERROR,
             RUNNING,
             BLOCKED,
             KILLED };

typedef struct P_rq {
      uint32_t pid;
      enum State state;
} p_rq;

void test_processes() {
      p_rq p_rqs[MAX_PROCESSES];
      uint8_t rq;
      uint8_t alive = 0;
      uint8_t action;

      while (1) {
            // Create MAX_PROCESSES processes
            for (rq = 0; rq < MAX_PROCESSES; rq++) {
                  p_rqs[rq].pid = my_create_process("testDummy");

                  if (p_rqs[rq].pid == 0) {
                        printfBR("Error creating process\n");
                        return;
                  } else {
                        p_rqs[rq].state = RUNNING;
                        alive++;
                  }
            }

            // Randomly kills, blocks or unblocks processes until every one has been killed
            while (alive > 0) {
                  for (rq = 0; rq < MAX_PROCESSES; rq++) {
                        action = GetUniform(2) % 2;
                        switch (action) {
                              case 0:
                                    if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
                                          if (my_kill(p_rqs[rq].pid) == 0) {
                                                printfBR("Error killing process\n");
                                                return;
                                          }
                                          p_rqs[rq].state = KILLED;
                                          alive--;
                                    }
                                    break;

                              case 1:
                                    if (p_rqs[rq].state == RUNNING) {
                                          if (my_block(p_rqs[rq].pid) == 0) {
                                                printfBR("Error blocking process\n");
                                                return;
                                          }
                                          p_rqs[rq].state = BLOCKED;
                                    }
                                    break;
                        }
                        sys_yield();
                  }


                  // Randomly unblocks processes
                  for (rq = 0; rq < MAX_PROCESSES; rq++) {
                        if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2) {
                              if (my_unblock(p_rqs[rq].pid) == 0) {
                                    printfBR("Error unblocking process\n");
                                    return;
                              }
                              p_rqs[rq].state = RUNNING;
                        }
                  }
            }
      }
}

static uint32_t my_create_process(char* name) {
      char* args[] = {name};
      return sys_loadApp(&endless_loop, 1,args,0);
}

static uint32_t my_kill(uint32_t pid) {
      return sys_kill(pid);
}

static uint32_t my_block(uint32_t pid) {
      return sys_block(pid);
}

static uint32_t my_unblock(uint32_t pid) {
      return sys_block(pid);
}

static void endless_loop(int argc, char** args) {
      while (1);
}