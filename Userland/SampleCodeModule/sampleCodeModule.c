#include <taskManager.h>
#include <systemCalls.h>
#include <stringLib.h>
#include <shell.h>


int main() {
      char* args[] = {"Shell"};
      sys_loadApp(&runShell, 1, args, 1,0);
      return 0;
}