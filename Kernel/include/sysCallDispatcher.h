#ifndef SYS_CALL_DISPATCHER
#define SYS_CALL_DISPATCHER

#include <stdint.h>

typedef struct
{
      uint64_t rax, rdi, rsi, rdx, r10, r8, r9;
} t_registers;

uint64_t sysCallDispatcher(t_registers * r);

#endif 