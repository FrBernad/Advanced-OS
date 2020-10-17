// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stddef.h>
#include <stdint.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <utils.h>

#include "test_util.h"

static void slowInc(int64_t *p, int64_t inc);
static void inc(int argc, char **argv);

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

int64_t global;  //shared memory

int test_sync() {
      uint64_t i;

      global = 0;

      printfBR("CREATING PROCESSES...(WITH SEM)\n");

      char *argv1[] = {"inc", "1", "1", "100000"};
      char *argv2[] = {"inc", "1", "-1", "100000"};

      for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
            sys_loadApp(&inc, 4, argv1, 0, 0);
            sys_loadApp(&inc, 4, argv2, 0, 0);
      }
      return 0;
}

int test_no_sync() {
      uint64_t i;

      global = 0;

      printfBR("CREATING PROCESSES...(WITHOUT SEM)\n");

      char *argv1[] = {"inc", "0", "1", "100000"};
      char *argv2[] = {"inc", "0", "-1", "100000"};

      for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
            sys_loadApp(&inc, 4, argv1, 0, 0);
            sys_loadApp(&inc, 4, argv2, 0, 0);
      }
      return 0;
}

static void slowInc(int64_t *p, int64_t inc) {
      int64_t aux = *p;
      aux += inc;
      sys_yield();
      *p = aux;
}

static void inc(int argc, char **argv) {
      int sem = atoi(argv[1]);
      int value = atoi(argv[2]);
      int N = atoi(argv[3]);
      int semIndex;
      uint64_t i;

      if (sem) {
            if ((semIndex = sys_sem_open(SEM_ID, 1)) == -1) {
                  printfBR("ERROR OPENING SEM\n");
                  return;
            }
      }

      for (i = 0; i < N; i++) {
            if (sem)
                  sys_sem_wait(semIndex);
            slowInc(&global, value);
            if (sem)
                  sys_sem_post(semIndex);
      }

      if (sem)
            sys_sem_close(semIndex);

      if (sem)
            printfBR("SYNC Final value: %d\n", global);
      else
            printfBR("NO SYNC Final value: %d\n", global);
}