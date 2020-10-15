#ifndef FREE_LIST_MM

#include <lib.h>
#include <memoryManager.h>
#include <stddef.h>
#include <stdint.h>
#include <utils.h>

#define MIN_ALLOC_LOG_2 6                  //64 B min aloc
#define MAX_LEVELS (30 - MIN_ALLOC_LOG_2)  ////1GB MB max aloc

#define BIN_POW(x) (1 << (x))

typedef struct list_t {
      uint8_t isFree;
      uint8_t level;
      struct list_t *prev, *next;
} list_t;

static void list_init(list_t *list);
static void list_push(list_t *list, list_t *entry);
static void list_remove(list_t *entry);
static list_t *list_pop(list_t *list);
static int list_is_empty(list_t *list);
static uint8_t powerof2(uint32_t num) ;

static void addToLevel(list_t *list, list_t *node, uint8_t level);

static uint8_t getLevel(uint32_t bytes);
static int getFirstAvailableLevel(uint8_t minLevel);
static list_t *getBuddy(list_t *node);
static list_t *getAdress(list_t *node);

//Global variables
static list_t *base;
static uint32_t maxMemSize;
static list_t levelsList[MAX_LEVELS];
static uint8_t levels;


void initMemoryManager(void *memBase, uint64_t totalSize) {

      base = memBase;
      maxMemSize = totalSize;
      levels = (int)log2(totalSize) - MIN_ALLOC_LOG_2 + 1;

      if (memBase == NULL)
            return;

      if (levels > MAX_LEVELS) 
            levels = MAX_LEVELS;

      for (size_t i = 0; i < levels; i++) {
            list_init(&levelsList[i]);
            levelsList[i].isFree = 0;
            levelsList[i].level = i;
      }

      addToLevel(&levelsList[levels - 1], base, levels - 1);
      printfBR("init buddyMM\n");
}

void *mallocBR(uint32_t bytes) {
      if (bytes == 0)
            return NULL;

      uint32_t memNeeded = bytes + sizeof(list_t);

      memNeeded += sizeof(list_t);

      if (memNeeded > maxMemSize)
            return NULL;

      uint8_t minLevel = getLevel(memNeeded);

      int availableLevel = getFirstAvailableLevel(minLevel);

      if (availableLevel == -1) {
            return NULL;
      }

      list_t *node;
      for (node = list_pop(&levelsList[availableLevel]); minLevel < availableLevel; availableLevel--) {
            node->level--;
            addToLevel(&levelsList[availableLevel - 1], getBuddy(node), availableLevel - 1);
      }
      node->isFree = 0;

      return (void *)(node + 1);
}

void freeBR(void *ptr) {
      if (ptr == NULL)
            return;

      list_t *insertp = (list_t *)ptr - 1;


      insertp->isFree = 1;

      list_t *buddy = getBuddy(insertp);

      while (insertp->level != levels - 1 && buddy->level == insertp->level && buddy->isFree) {
            list_remove(buddy);

            insertp = getAdress(insertp);
            insertp->level++;

            buddy = getBuddy(insertp);
      }
      list_push(&levelsList[insertp->level], insertp);
}

static void addToLevel(list_t *list, list_t *node, uint8_t level) {
      node->isFree = 1;
      node->level = level;
      list_push(list, node);
}

static list_t *getBuddy(list_t *node) {
      uint8_t level = node->level;
      uintptr_t currentOffset = (uintptr_t)node - (uintptr_t)base;
      uintptr_t newOffset = currentOffset ^ BIN_POW(MIN_ALLOC_LOG_2 + level);

      return (list_t *)(newOffset + (uintptr_t)base);
}

static int getFirstAvailableLevel(uint8_t minLevel) {
      uint8_t selectedLevel;
      for (selectedLevel = minLevel; selectedLevel < levels && list_is_empty(&levelsList[selectedLevel]);selectedLevel++);
      if (selectedLevel > levels) {
            return -1;
      }
      return selectedLevel;
}

static uint8_t getLevel(uint32_t bytes) {
      uint8_t aux = log2(bytes);
      if (aux < MIN_ALLOC_LOG_2)
            return 0;

      aux -= MIN_ALLOC_LOG_2;
      if (powerof2(bytes)) //se es potencia me alcanza sino necesito mas
            return aux;

      return aux + 1;
}

static list_t *getAdress(list_t *node) {
      uint8_t level = node->level;
      uintptr_t mask = BIN_POW(MIN_ALLOC_LOG_2 + level);
      mask = ~mask;

      uintptr_t currentOffset = (uintptr_t)node - (uintptr_t)base;
      uintptr_t newOffset = currentOffset & mask;

      return (list_t *)(newOffset + (uintptr_t)base);
}

static uint8_t powerof2(uint32_t num) {
      return num && !(num & (num - 1));
}

static void list_init(list_t *list) {
      list->prev = list;
      list->next = list;
}

static void list_push(list_t *list, list_t *entry) {
      list_t *prev = list->prev;
      entry->prev = prev;
      entry->next = list;
      prev->next = entry;
      list->prev = entry;
}

static void list_remove(list_t *entry) {
      list_t *prev = entry->prev;
      list_t *next = entry->next;
      prev->next = next;
      next->prev = prev;
}

static list_t *list_pop(list_t *list) {
      list_t *back = list->prev;
      if (back == list) return NULL;
      list_remove(back);
      return back;
}

static int list_is_empty(list_t *list) {
      return list->prev == list;
}

#endif
