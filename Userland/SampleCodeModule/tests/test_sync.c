#include <stddef.h>
#include <stdint.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <utils.h>

#include "test_util.h"
static uint64_t my_create_process_sync(char *name, void (*function)(int, char **));
static int32_t my_sem_open_sync(char *sem_id, uint64_t initialValue);
static int my_sem_wait_sync(int sem_id);
static int my_sem_post_sync(int sem_id);
static int my_sem_close_sync(int sem_id);
static void my_process_inc();
static void my_process_dec();
static void my_process_inc_no_sem();
static void my_process_dec_no_sem();
static void slowInc(int64_t *p, int64_t inc);

#define N 100000
#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global;  //shared memory

void test_sync() {
      uint64_t i;

      global = 0;

      printfBR("CREATING PROCESSES...\n");

      for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
            my_create_process_sync("my_process_inc", (void (*)(int, char **))my_process_inc);
            my_create_process_sync("my_process_dec", (void (*)(int, char **))my_process_dec);
      }

      // The last one should printfBR 0
}

static void my_process_inc() {
      uint64_t i;

      int semid = my_sem_open_sync(SEM_ID, 1);
      if (semid == -1) {
            printfBR("ERROR OPENING SEM\n");
            return;
      }

      for (i = 0; i < 500; i++) {
            my_sem_wait_sync(semid);
            slowInc(&global, 1);
            my_sem_post_sync(semid);
      }

      my_sem_close_sync(semid);

      printfBR("Final value: %d\n", global);
}

static void my_process_dec() {
      uint64_t i;

      int semid = my_sem_open_sync(SEM_ID, 1);
      if (semid == -1) {
            printfBR("ERROR OPENING SEM\n");
            return;
      }

      for (i = 0; i < 500; i++) {
            my_sem_wait_sync(semid);
            slowInc(&global, -1);
            my_sem_post_sync(semid);
      }

      my_sem_close_sync(semid);

      printfBR("Final value: %d\n", global);
}

void test_no_sync() {
      uint64_t i;

      global = 0;

      printfBR("CREATING PROCESSES...\n");

      for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
            my_create_process_sync("my_process_inc_no_sem", (void (*)(int, char **))my_process_inc_no_sem);
            my_create_process_sync("my_process_dec_no_sem", (void (*)(int, char **))my_process_dec_no_sem);
      }

      // The last one should not print 0
}

static void my_process_inc_no_sem() {
      printfBR("starting");
      uint64_t i;
      for (i = 0; i < N; i++) {
            slowInc(&global, 1);
      }

      printfBR("Final value: %d\n", global);
}

static void my_process_dec_no_sem() {
      uint64_t i;
      for (i = 0; i < N; i++) {
            slowInc(&global, -1);
      }

      printfBR("Final value: %d\n", global);
}

static void slowInc(int64_t *p, int64_t inc) {
      int64_t aux = *p;

      // Simula mala suerte
      if (GetUniform(100) % 31 == 1)
            sys_yield();

      aux += inc;
      *p = aux;
}






static uint64_t my_create_process_sync(char *name, void (*function)(int, char **)) {
      char *argv[] = {name};
      return sys_loadApp(function, 1, argv, 0);
}

static int32_t my_sem_open_sync(char *sem_id, uint64_t initialValue) {
      return sys_sem_open(sem_id, initialValue);
}

static int my_sem_wait_sync(int sem_id) {
      return sys_sem_wait(sem_id);
}

static int my_sem_post_sync(int sem_id) {
      return sys_sem_post(sem_id);
}

static int my_sem_close_sync(int sem_id) {
      return sys_sem_close(sem_id);
}