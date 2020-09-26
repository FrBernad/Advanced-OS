#include <taskManager.h>
#include <systemCalls.h>
#include <shell.h>


int main() {
      char* args[] = {"Shell"};
      syscall(LOAD_APP, (uint64_t)&runShell, 1, (uint64_t)args, 0, 0, 0);

      return 0;
}