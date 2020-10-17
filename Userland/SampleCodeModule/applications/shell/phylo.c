// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <phylo.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <utils.h>

#define MAX_PHYLO 6
#define MIN_PHYLO 2
#define MAX_LENGTH 20
#define EATING 1
#define WAITING 2
#define SEM_NAME "PHYLO_"

typedef struct phylo {
      uint8_t isRunning;
      uint64_t pid;
      uint8_t hasRightStick;
      uint8_t state;
} t_phylo;

static int initPhylo(int phyloCount);
static void leftPhylosopher(int argc, char** args);
static void rightPhylosopher(int argc, char** args);
static void printTable(int index, int state);
static void awaitPhylos();
static void addPhylosopher();
static void waitForPhylo(t_phylo* phylo);
static void removePhylosopher();

static int chopsticks[MAX_PHYLO];
static t_phylo phylos[MAX_PHYLO];
static char table[MAX_PHYLO + 1];
static int phyloCount;

void phylo(int argc, char** args) {
      if (argc != 2) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      int error = 0;
      phyloCount = strToInt(args[1], &error);
      if (error) {
            printfBR("Invalid number of phylos\n");
            return;
      }

      if (phyloCount > MAX_PHYLO)
            phyloCount = MAX_PHYLO;
      else if (phyloCount < MIN_PHYLO)
            phyloCount = MIN_PHYLO;

      if (initPhylo(phyloCount) == -1) {
            printfBR("Error initializing phylo\n");
            return;
      }

      int c;
      while ((c = getchar()) != -1) {
            switch ((char)c) {
                  case 'a':
                        addPhylosopher();
                        break;

                  case 'r':
                        removePhylosopher();
                        break;
            }
      }
      awaitPhylos();
}

static int initPhylo(int phyloCount) {
      char semName[MAX_LENGTH]={0};
      char semID[MAX_LENGTH];
      strcpy(semName,SEM_NAME);

      for (int i = 0; i < phyloCount; i++) {
            uintToBase(i, semID, 10);
            strcat(semName, semID);
            chopsticks[i] = sys_sem_open(semName, 1);
            table[i] = '.';
            if (chopsticks[i] == -1)
                  return -1;

            phylos[i].state = WAITING;
            phylos[i].isRunning = 1;
            phylos[i].hasRightStick = 0;
            char* argv[] = {semName, semID};
            printfBR("Phylo %d arrived to the table\n", i+1);
            phylos[i].pid = sys_loadApp((i % 2 == 0) ? &leftPhylosopher : &rightPhylosopher, 2, argv, 0, 0);
      }

      return 1;
}

static void addPhylosopher() {
      if (phyloCount >= MAX_PHYLO) {
            return;
      }

      char semName[MAX_LENGTH]={0};
      char semID[MAX_LENGTH];

      uint16_t count = phyloCount;

      strcpy(semName,SEM_NAME);

      uintToBase(count, semID, 10);
      strcat(semName, semID);

      t_phylo* phylo = &phylos[count - 1];

      waitForPhylo(phylo);

      chopsticks[count] = sys_sem_open(semName, 1);
      if (chopsticks[count] == -1) {
            printfBR("error opening sem\n");
            return;
      }

      table[count] = '.';
      phylos[count].state = WAITING;
      phylos[count].isRunning = 1;

      phyloCount++;

      sys_unblock(phylo->pid);

      char* argv[] = {semName, semID};
      phylos[count].pid = sys_loadApp((count % 2 == 0) ? &leftPhylosopher : &rightPhylosopher, 2, argv, 0, 0);

      printfBR("Phylo %d arrived to the table\n", count + 1);
}

static void removePhylosopher() {
      if (phyloCount <= MIN_PHYLO)
            return;

      int count = phyloCount - 1;

      t_phylo* toRemove = &phylos[count];
      t_phylo* newLast = &phylos[count - 1];

      toRemove->isRunning = 0;

      waitForPhylo(newLast);

      sys_wait(toRemove->pid);

      phyloCount--;

      table[count] = 0;

      if (sys_sem_close(chopsticks[count]) == -1) {
            printfBR("error closing sem\n");
            return;
      }
      printfBR("Phylo %d left the table\n", count + 1);
      sys_unblock(newLast->pid);
}

static void waitForPhylo(t_phylo* phylo) {
      while (1) {
            if (sys_block(phylo->pid) == 1)
                  sys_yield();

            else if (phylo->hasRightStick) {
                  sys_unblock(phylo->pid);
                  sys_yield();
            } else
                  return;
      }
}

static void rightPhylosopher(int argc, char** args) {
      int error = 0;
      int phyloID = strToInt(args[1], &error);
      if (error) {
            printfBR("Invalid phylo id\n");
            return;
      }

      while (phylos[phyloID].isRunning) {
            sleep(2);

            phylos[phyloID].hasRightStick = 1;
            sys_sem_wait(chopsticks[(phyloID + 1) % phyloCount]);  //wait right chopstick

            sys_sem_wait(chopsticks[phyloID]);  //wait left chopstick

            printTable(phyloID, EATING);
            sleep(1);

            sys_sem_post(chopsticks[phyloID]);

            sys_sem_post(chopsticks[(phyloID + 1) % phyloCount]);
            phylos[phyloID].hasRightStick = 0;

            table[phyloID] = '.';
      }
}

static void leftPhylosopher(int argc, char** args) {
      int error = 0;
      int phyloID = strToInt(args[1], &error);
      if (error) {
            printfBR("Invalid phylo id\n");
            return;
      }

      while (phylos[phyloID].isRunning) {
            sleep(2);

            sys_sem_wait(chopsticks[phyloID]);  //wait left chopstick

            phylos[phyloID].hasRightStick = 1;
            sys_sem_wait(chopsticks[(phyloID + 1) % phyloCount]);  //wait right chopstick

            printTable(phyloID, EATING);
            sleep(1);

            sys_sem_post(chopsticks[(phyloID + 1) % phyloCount]);
            phylos[phyloID].hasRightStick = 0;

            sys_sem_post(chopsticks[phyloID]);

            table[phyloID] = '.';
      }
}

static void printTable(int index, int state) {
      table[index] = (state == WAITING ? '.' : 'E');
      printfBR("%s\n", table);
}

static void awaitPhylos() {
      for (int i = 0; i < phyloCount; i++){
            phylos[i].isRunning=0;
      }

      for (int i = 0; i < phyloCount; i++) {
            sys_wait(phylos[i].pid);
      }

      for (int i = 0; i < phyloCount; i++){
            printfBR("Phylo %d left the table\n", i + 1);
            sys_sem_close(chopsticks[i]);
      }

      for (int i = 0; i < MAX_PHYLO + 1; i++) {
            table[i] = 0;
      }

      phyloCount = 0;
}