#ifndef BUDDY_MM
#define FREE_LIST_MM

#include <memoryManager.h>
#include <stddef.h>

#define BLOCK_SIZE sizeof(node)

typedef uint64_t Align;
union node {
      struct {
            union node *next;
            uint32_t size;
      } s;
      Align x;
};
typedef union node node;

static void joinBlocks(node *left, node *right);

static uint64_t memorySize;
static node *first;
static node *base;

void initMemoryManager(void *memBase, uint64_t memSize){
      base = memBase;
      memorySize=memSize;
      first = (node *)base;
      first->s.size = memorySize / BLOCK_SIZE;
      first->s.next = NULL;
}

void *mallocBR(uint32_t nbytes) {
      if (nbytes == 0)
            return NULL;

      node *current, *prev = first;

      uint32_t nblocks = (nbytes + BLOCK_SIZE - 1) / BLOCK_SIZE + 1;

      for (current = prev; current != NULL; prev = current, current = current->s.next) {
            if (current->s.size >= nblocks) {
                  if (current->s.size == nblocks) {
                        if (current == first)
                              first = current->s.next;
                        else
                              prev->s.next = current->s.next;
                  } else {
                        current->s.size -= nblocks;
                        current += current->s.size;
                        current->s.size = nblocks;
                  }
                  return (void *)(current + 1);
            }
      }
      return NULL;
}

void freeBR(void *ptr) {
      if (ptr == NULL)
            return;

      node *insertp = (node *)ptr - 1;

      if (insertp < base || insertp >= (base + memorySize * BLOCK_SIZE) || ((char *)insertp - (char *)base) % BLOCK_SIZE != 0) {
            return;
      }

      if (first == NULL) {
            first = insertp;
            first->s.next = NULL;
            return;
      }

      if (insertp < first) {
            joinBlocks(insertp, first);
            first = insertp;
            return;
      }

      node *currentp;
      for (currentp = first; currentp->s.next != NULL && !(currentp < insertp && insertp < currentp->s.next); currentp = currentp->s.next)
            ;

      joinBlocks(insertp, currentp->s.next);
      joinBlocks(currentp, insertp);
}

static void joinBlocks(node *left, node *right) {
      if (left == NULL || (right != NULL && left >= right)) {
            return;
      }

      if (left + left->s.size == right) {
            left->s.size += right->s.size;
            left->s.next = right->s.next;
      } else
            left->s.next = right;
}

#endif