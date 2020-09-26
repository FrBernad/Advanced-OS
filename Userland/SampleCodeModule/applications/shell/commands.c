#include <RTCTime.h>
#include <commands.h>
#include <cpuInfo.h>
#include <lib.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <utils.h>

static void memToString(char* buffer, uint8_t* mem, int bytes);
static void invalidOpcodeTrigger();
static void zeroDivisionTrigger();

//devuelve el tiempo acutal del sistema
void time(int argc, char** args, t_shellData* shellData) {
      if (argc != 0) {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }

      uint8_t hours = syscall(RTC_TIME, HOURS, 0, 0, 0, 0, 0);
      uint8_t mins = syscall(RTC_TIME, MINUTES, 0, 0, 0, 0, 0);
      uint8_t secs = syscall(RTC_TIME, SECONDS, 0, 0, 0, 0, 0);
      printfBR(" >Current time: %d:%d:%d\n\n",hours,mins,secs);
}

//devuelve el modelo y vendedor del cpu
void cpuInfo(int argc, char** args, t_shellData* shellData) {
      if (argc != 0) {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }
      char vendor[13] = {0};
      t_cpuInfo cpuInfo = {vendor, 0};
      cpuVendor(cpuInfo.cpuVendor);
      cpuInfo.model = cpuModel();
      printfBR(" > CPU Vendor: %s", cpuInfo.cpuVendor);
      printfBR(" > CPU model: %s\n\n", cpuInfo.model);
}

//Hace un dump de 32 bytes de memria a partir de la direccion pedida
void printmem(int argc, char** args, t_shellData* shellData) {
      if (argc != 1) {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }

      int error = 0;
      uint64_t memDir = strToHex(args[0], &error);
      if (error) {
            printStringLn("Invalid argument for function printmem (must be a hex value).");
            putchar('\n');
            return;
      }

      int bytes = 32;

      uint8_t memData[bytes];
      syscall(GET_MEM, memDir, (uint64_t)memData, 0, 0, 0, 0);

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
void cpuTemp(int argc, char** args, t_shellData* shellData) {
      if (argc != 0) {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }
      printfBR("CPU temp: %d C\n", syscall(TEMP, 0, 0, 0, 0, 0, 0));
}

//causa una excepcion de dividir por cero
void checkZeroException(int argc, char** args, t_shellData* shellData) {
      if (argc != 0) {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }
      char* args2[] = {"Zero Division Trigger"};
      syscall(LOAD_APP, (uint64_t)&zeroDivisionTrigger, 1, (uint64_t)args2, 0, 0, 0);
}

static void zeroDivisionTrigger(){
      int a = 0;
      int b = 20 / a;
      if (b == 0) {
      }
}

//causa una excepcion de tipo invalid opcode
void checkInvalidOpcodeException(int argc, char** args, t_shellData* shellData) {
      if (argc != 0) {
            printfBR("Invalid ammount of arguments.\n");
            return;
      }
      char* args2[] = {"Invalid Opcode Trigger"};
      syscall(LOAD_APP, (uint64_t)&invalidOpcodeTrigger, 1, (uint64_t)args2, 0, 0, 0);
}

static void invalidOpcodeTrigger(){
      __asm__("ud2");  // https://hjlebbink.github.io/x86doc/html/UD2.html
}

//Muestra los argumentos pasados al comando
void showArgs(int argc, char** args, t_shellData* shellData) {
      for (int i = 0; i < argc && i < MAX_ARGS; i++) {
            printfBR("arg[%d]=%s\n",i,args[i]);
      }
      putchar('\n');
}

//Muestra los procesos
void ps(int argc, char** args, t_shellData* shellData) {
      if(argc!=0){
            printfBR("Invalid ammount of arguments.\n");
      }
      syscall(PS, 0, 0, 0, 0, 0, 0);
}

// Imprime su ID con un saludo cada una determinada cantidad de segundos.
void loop(int argc, char** args, t_shellData* shellData){
      if (argc != 0) {
            printfBR("Invalid ammount of arguments.\n");
      }
      syscall(LOOP, 0, 0, 0, 0, 0, 0);
}

// Mata un proceso dado su ID.
void kill(int argc, char** args, t_shellData* shellData){
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
      }
      int error = 0;
      uint64_t pid = strToInt(args[0],&error);
      if(error){
            printStringLn("Invalid pid");
            return;
      }
      syscall(KILL,pid,0,0,0,0,0);
}

// Cambia la prioridad de un proceso dado su ID y la nueva prioridad.
void nice(int argc, char** args, t_shellData* shellData){
      if (argc != 2) {
            printfBR("Invalid ammount of arguments.\n");
      }
      int error = 0;
      uint64_t pid = strToInt(args[0], &error);
      if (error) {
            printStringLn("Invalid pid");
            return;
      }
      uint64_t priority = strToInt(args[1], &error);
      if (error) {
            printStringLn("Invalid pid");
            return;
      }
      syscall(NICE, pid, priority, 0, 0, 0, 0);
}

// Cambia el estado de un proceso entre bloqueado y listo dado su ID.
void block(int argc, char** args, t_shellData* shellData){
      if (argc != 1) {
            printfBR("Invalid ammount of arguments.\n");
      }
      int error = 0;
      uint64_t pid = strToInt(args[0], &error);
      if (error) {
            printStringLn("Invalid pid");
            return;
      }
      syscall(BLOCK, pid, 0, 0, 0, 0, 0);
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
