// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <RTCTime.h>
#include <commands.h>
#include <cpuInfo.h>
#include <lib.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <tests.h>
#include <utils.h>

static void memToString(char* buffer, uint8_t* mem, int bytes);
static void invalidOpcodeTrigger();
static void zeroDivisionTrigger();
static void loopFunction();

//devuelve el tiempo acutal del sistema
void time(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }

      uint8_t hours = sys_RTCTime(HOURS);
      uint8_t mins = sys_RTCTime(MINUTES);
      uint8_t secs = sys_RTCTime(SECONDS);
      printfBR(" >Current time: %d:%d:%d\n", hours, mins, secs);
}

//devuelve el modelo y vendedor del cpu
void cpuInfo(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      char vendor[13] = {0};
      t_cpuInfo cpuInfo = {vendor, 0};
      cpuVendor(cpuInfo.cpuVendor);
      cpuInfo.model = cpuModel();
      printfBR(" > CPU Vendor: %s\n", cpuInfo.cpuVendor);
      printfBR(" > CPU model: %d\n\n", cpuInfo.model);
}

//Hace un dump de 32 bytes de memria a partir de la direccion pedida
void printmem(int argc, char** args) {
      if (argc != 2) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }

      int error = 0;
      uint64_t memDir = strToHex(args[1], &error);
      if (error) {
            printfBR("Invalid argument for function printmem (must be a hex value).\n");
            return;
      }

      int bytes = 32;

      uint8_t memData[bytes];
      sys_getMem(memDir, memData);

      char byteStr[bytes * 2];
      memToString(byteStr, memData, bytes);

      printString(" >Data dump:");
      for (int i = 0; i < 32; i++) {
            if (i % 4 == 0) {
                  putchar('\n');
                  printString("   -[0x");
                  printHex(memDir);
                  printString("]: ");
                  memDir += 4;
            }
            if (i % 2 == 0) {
                  putcharWC(byteStr[i], BLACK, BLUE);
                  putcharWC(byteStr[i + 1], BLACK, BLUE);
                  putchar(' ');
            } else {
                  putchar(byteStr[i]);
                  putchar(byteStr[i + 1]);
                  putchar(' ');
            }
      }
      putchar('\n');
      putchar('\n');
}


//Imprime la temperatura actual del cpu
void cpuTemp(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      printfBR("CPU temp: %d C\n", sys_temp());
}

//causa una excepcion de dividir por cero
void checkZeroException(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      zeroDivisionTrigger();
}

static void zeroDivisionTrigger() {
      int a = 0;
      int b = 20 / a;
      if (b == 0) {
      }
}

//causa una excepcion de tipo invalid opcode
void checkInvalidOpcodeException(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      invalidOpcodeTrigger();
}

static void invalidOpcodeTrigger() {
      __asm__("ud2");  // https://hjlebbink.github.io/x86doc/html/UD2.html
}

//Muestra los argumentos pasados al comando
void showArgs(int argc, char** args) {
      for (int i = 0; i < argc && i < MAX_ARGS; i++) {
            printfBR("arg[%d]=%s\n", i, args[i]);
      }
      putchar('\n');
}

//Muestra los procesos
void ps(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      sys_ps();
}

// Imprime su ID con un saludo cada una determinada cantidad de segundos.
void loop(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      loopFunction();
}

static void loopFunction() {
      uint64_t pid = sys_getPID();
      while (1) {
            waitTicks(8);
            printfBR("%d", pid);
      }
}

// Mata un proceso dado su ID.
void kill(int argc, char** args) {
      if (argc != 2) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      int error = 0;
      uint64_t pid = strToInt(args[1], &error);
      if (error) {
            printStringLn("Invalid pid");
            return;
      }
      sys_kill(pid);
}

// Cambia la prioridad de un proceso dado su ID y la nueva prioridad.
void nice(int argc, char** args) {
      if (argc != 3) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      int error = 0;
      uint64_t pid = strToInt(args[1], &error);
      if (error) {
            printStringLn("Invalid pid");
            return;
      }
      uint64_t priority = strToInt(args[2], &error);
      if (error) {
            printStringLn("Invalid pid");
            return;
      }
      sys_nice(pid, priority);
}

// Cambia el estado de un proceso entre bloqueado y listo dado su ID.
void block(int argc, char** args) {
      if (argc != 2) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      int error = 0;
      uint64_t pid = strToInt(args[1], &error);
      if (error) {
            printStringLn("Invalid pid");
            return;
      }
      sys_block(pid);
}

void unblock(int argc, char** args){
      if (argc != 2) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      int error = 0;
      uint64_t pid = strToInt(args[1], &error);
      if (error) {
            printStringLn("Invalid pid");
            return;
      }
      sys_unblock(pid);
}

static void memToString(char* buffer, uint8_t* mem, int bytes) {
      for (int i = 0; i < bytes * 2; i++) {
            if (mem[i] <= 0xF) {
                  buffer[i] = '0';
                  uintToBase(mem[i], buffer + i + 1, 16);
            } else {
                  uintToBase(mem[i], buffer + i, 16);
            }
      }
}

void testMM(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      test_mm();
}

void testProcesses(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      test_processes();
}

void testPriority(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      test_priority();
}

void testSync(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      test_sync();
}

void testNoSync(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      test_no_sync();
}

void dumpSemaphores(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      sys_dumpSemaphores();
}

void dumpPipes(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      sys_dumpPipes();
}

void cat(int argc, char** args){
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }

      int c;
      while((c=getchar())!=-1){
            putchar(c);
      }
}

//counts the lines recieved from input
void wc(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }

      uint16_t counter = 1;
      int c;

      while ((c = getchar()) != -1) {
            putchar(c);
            if ((char)c == '\n')
                  counter++;            
      }
      printfBR("total lines: %d\n",counter);
}

void filter(int argc, char** args){
        if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }

      int c;
      while ((c = getchar()) != -1) {
            if (!IS_VOWEL(c))
                  putchar(c);
      }

      printfBR("\n");
}

void dumpMM(int argc, char** args) {
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      sys_dumpMM();
}
