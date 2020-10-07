#include <stddef.h>
#include <stdint.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <utils.h>

#define MINOR_WAIT 1000000*5
#define TOTAL_PROCESSES 3

static uint64_t my_getpid();
static uint32_t my_create_process(char* name);
static uint64_t my_nice(uint64_t pid, uint64_t newPrio);
static uint32_t my_kill(uint32_t pid);
static uint32_t my_block(uint32_t pid);
static uint32_t my_unblock(uint32_t pid);
static void bussy_wait(uint64_t n);
static void endless_loop();

void test_priority() {
      uint64_t pids[TOTAL_PROCESSES];
      uint64_t i;

      for (i = 0; i < TOTAL_PROCESSES; i++)
            pids[i] = my_create_process("endless_loop");

      sleep(1);
      printfBR("\nCHANGING PRIORITIES...\n");

      for (i = 0; i < TOTAL_PROCESSES; i++) {
            switch (i % 3) {
                  case 0:
                        my_nice(pids[i], 0);  //lowest priority
                        break;
                  case 1:
                        my_nice(pids[i], 1);  //medium priority
                        break;
                  case 2:
                        my_nice(pids[i], 2);  //highest priority
                        break;
            }
      }

      sleep(1);
      printfBR("\nBLOCKING...\n");

      for (i = 0; i < TOTAL_PROCESSES; i++)
            my_block(pids[i]);

      printfBR("CHANGING PRIORITIES WHILE BLOCKED...\n");
      for (i = 0; i < TOTAL_PROCESSES; i++) {
            switch (i % 3) {
                  case 0:
                        my_nice(pids[i], 1);  //medium priority
                        break;
                  case 1:
                        my_nice(pids[i], 1);  //medium priority
                        break;
                  case 2:
                        my_nice(pids[i], 1);  //medium priority
                        break;
            }
      }

      printfBR("UNBLOCKING...\n");

      for (i = 0; i < TOTAL_PROCESSES; i++)
            my_unblock(pids[i]);

      sleep(1);
      printfBR("\nKILLING...\n");

      for (i = 0; i < TOTAL_PROCESSES; i++)
            my_kill(pids[i]);
}

static uint64_t my_getpid() {
      return sys_getPID();
}

static uint32_t my_create_process(char* name) {
      char* args[] = {name};
      return sys_loadApp(&endless_loop, 1, args);
}

static uint64_t my_nice(uint64_t pid, uint64_t newPrio) {
      return sys_nice(pid, newPrio);
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

static void bussy_wait(uint64_t n) {
      uint64_t i;
      for (i = 0; i < n; i++)
            ;
}

static void endless_loop() {
      uint64_t pid = my_getpid();

      while (1) {
            printfBR("%d ", pid);
            bussy_wait(MINOR_WAIT);
      }
}