#include <stddef.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <tests.h>
#include <utils.h>
#include "test_util.h"

#define MAX_BLOCKS 128
#define MAX_MEMORY (1024 * 1024 * 60) 

typedef struct MM_rq {
      void *address;
      uint64_t size;
} mm_rq;

void test_mm() {
      mm_rq mm_rqs[MAX_BLOCKS];
      uint8_t rq;
      uint32_t total;
      printfBR("Starting memory manager test!\n");
      while (1) {
            rq = 0;
            total = 0;

            // Request as many blocks as we can
            while (rq < MAX_BLOCKS && total < MAX_MEMORY) {
                  mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
                  mm_rqs[rq].address = sys_malloc(mm_rqs[rq].size);
                  if (mm_rqs[rq].address == NULL)
                        printfBR("Malloc returned NULL\n");

                  total += mm_rqs[rq].size;
                  rq++;
            }

            // Set
            uint32_t i;
            for (i = 0; i < rq; i++)
                  if (mm_rqs[i].address != NULL)
                        memset(mm_rqs[i].address, i, mm_rqs[i].size);

            // Check
            for (i = 0; i < rq; i++)
                  if (mm_rqs[i].address != NULL)
                        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
                              printfBR("ERROR!\n");

            // Free
            for (i = 0; i < rq; i++)
                  if (mm_rqs[i].address != NULL){
                        sys_free(mm_rqs[i].address);
                  }
      }
}