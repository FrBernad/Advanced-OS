#include <commands.h>
#include <keys.h>
#include <lib.h>
#include <registers.h>
#include <shell.h>
#include <stdint.h>
#include <stringLib.h>
#include <systemCalls.h>
#include <utils.h>
#include <phylo.h>

static void initShell(t_shellData* shellData);
static void shellText(t_shellData* shellData);
static void processCommand(t_shellData* shellData);
static void processChar(char c, t_shellData* shellData);
static int processPipe(int pipeIndex, char** argv, int argc, int fg);
static int getCommand(char* commandName);
static int getPipeIndex(int argc, char** argv);
static void waitForPipes(uint32_t pids[2], int pipe, int fg);

static char* regNames[] = {"R15: ", "R14: ", "R13: ", "R12: ", "R11: ", "R10: ", "R9: ",
                           "R8: ", "RSI: ", "RDI: ", "RBP: ", "RDX: ", "RCX: ", "RBX: ",
                           "RAX: ", "RIP: ", "RSP: "};

static t_shellData shell;
static int pipeID;

void runShell(int argc, char* argv[]) {
      initShell(&shell);
      int c;
      while (1) {
            c = getchar();
            processChar(c, &shell);
      }
}

//inicia la shell y todas sus estructuras
static void initShell(t_shellData* shellData) {
      t_command commandsData[] = {
          {&help, "help", "shows the list of commands and their use"},
          {&inforeg, "inforeg", "prints the value of all the registers on screen, press ctrl + s to update values"},
          {&printmem, "printmem", "recieves an hexadecimal direction and makes a memory dump of 32 bytes on screen"},
          {&time, "time", "prints the current system time on screen"},
          {&cpuInfo, "cpuInfo", "prints the processor brand and model on screen"},
          {&cpuTemp, "cpuTemp", "prints the current processor temperature on screen"},
          {&changeUsername, "changeUsername", "changes the shell prompt username"},
          {&checkZeroException, "checkZeroException", "triggers a zero division exception"},
          {&checkInvalidOpcodeException, "checkInvalidOpcodeException", "triggers an invalid opcode exception"},
          {&showArgs, "showArgs", "prints the arguments passed to this command"},
          {&ps, "ps", "reports the status of the current processes"},
          {&loop, "loop", "loops process"},
          {&kill, "kill", "kills process with the given pid"},
          {&nice, "nice", "changes the priority of process with given pid"},
          {&block, "block", "blocks process with given pid"},
          {&unblock, "unblock", "unblocks process with given pid"},
          {&testProcesses, "testProcesses", "tests scheduler process creation"},
          {&testPriority, "testPriority", "tests scheduler priority"},
          {&testSync, "testSync", "tests sem sync"},
          {&testNoSync, "testNoSync", "tests sem sync, should fail"},
          {&testMM, "testMM", "tests memory manager"},
          {&dumpPipes, "pipe", "dump active pipes"},
          {&dumpSemaphores, "sem", "dump active semaphores"},
          {&wc, "wc", "counts the lines recieved from input"},
          {&filter, "filter", "filters the vocals recieved from input"},
          {&cat, "cat", "prints characters from input"},
          {&phylo, "phylo", "phylosphers dining problem, recieves number of initial phylosophers (max 6, min 2)"}
      };

      for (int i = 0; i < COMMANDS; i++) {
            shellData->commands[i].command = commandsData[i].command;
            shellData->commands[i].name = commandsData[i].name;
            shellData->commands[i].description = commandsData[i].description;
      }

      cleanBuffer(&shellData->buffer);
      strcpy("USER", shellData->username);
      shellText(shellData);
}

//procesa el caracter recibido actua segun el mismo
static void processChar(char c, t_shellData* shellData) {
      if (c != 0) {
            switch (c) {
                  case CLEAR_SCREEN:
                        sys_clear();
                        cleanBuffer(&shellData->buffer);
                        shellText(shellData);
                        break;
                  case '\n':
                        putchar('\n');
                        processCommand(shellData);
                        cleanBuffer(&shellData->buffer);
                        shellText(shellData);
                        break;
                  case '\b':
                        if (shellData->buffer.index > 0) {
                              shellData->buffer.buffer[--shellData->buffer.index] = 0;
                              deletechar();
                        }
                        break;

                  default:
                        if (shellData->buffer.index < BUFFER_SIZE) {
                              shellData->buffer.buffer[shellData->buffer.index++] = c;
                              putchar(c);
                        }
            }
      }
}

//procesa el comando, tokenizando lo ingresado.
static void processCommand(t_shellData* shellData) {
      int argc = 0;
      char* argv[MAX_ARGS] = {0};
      uint8_t fg = 1;

      argc = tokenizeBuffer(' ', argv, shellData->buffer.buffer, MAX_ARGS);

      int pipeIndex = getPipeIndex(argc, argv);

      if (pipeIndex == 0 || pipeIndex==argc) {
            printfBR("Invalid use of pipe \n");
            return;
      }

      if (argv[argc - 1][0] == '&') {
            fg = 0;
            argc--;
      }

      if (pipeIndex != -1) {
            if (processPipe(pipeIndex, argv, argc, fg) == -1) {
                  printfBR("Invalid commands for pipe \n");
                  return;
            }
            return;
      }

      int commandIndex = getCommand(argv[0]);

      if (commandIndex == -1) {
            printStringLn("Invalid command");
            return;
      }

      sys_loadApp(shellData->commands[commandIndex].command, argc, argv, fg, 0);
}

static int processPipe(int pipeIndex, char** argv, int argc, int fg) {
      char* auxArgv[MAX_ARGS];
      int auxArgc = 0;
      uint16_t fd[2];
      uint32_t pids[2];

      static char* shellPipeName = "_shellPipe";

      for (int i = pipeIndex + 1, j = 0; i < argc; i++) {
            auxArgv[j++] = argv[i];
            auxArgc++;
      }

      int commandIndex = getCommand(auxArgv[0]);

      if (commandIndex == -1)
            return -1;

      char pipeName[20];

      uintToBase(pipeID++, pipeName, 10);
      strcat(pipeName, shellPipeName);

      int pipe = sys_popen(pipeName);

      if (pipe == -1) {
            printfBR("Error creating pipe");
            return -1;
      }

      fd[0] = pipe;  //fd[0]: in, fd[1]: out
      fd[1] = 0;

      pids[0] = sys_loadApp(shell.commands[commandIndex].command, auxArgc, auxArgv, 0, fd);

      auxArgc = 0;

      for (int i = 0; i < pipeIndex; i++) {
            auxArgv[i] = argv[i];
            auxArgc++;
      }

      commandIndex = getCommand(auxArgv[0]);

      if (commandIndex == -1)
            return -1;

      fd[0] = 0;
      fd[1] = pipe;

      pids[1] = sys_loadApp(shell.commands[commandIndex].command, auxArgc, auxArgv, fg, fd);
      waitForPipes(pids, pipe, fg);

      return 1;
}

static void waitForPipes(uint32_t pids[2], int pipe, int fg) {
      int a = -1;
      if (fg == 0) 
            sys_wait(pids[1]);
      sys_writePipe(pipe, (char*)&a);
      sys_wait(pids[0]);
      sys_closePipe(pipe);
}

static int getCommand(char* commandName) {
      for (int i = 0; i < COMMANDS; i++) {
            if (stringcmp(shell.commands[i].name, commandName) == 0) {
                  return i;
            }
      }
      return -1;
}

static int getPipeIndex(int argc, char** argv) {
      for (int i = 0; i < argc; i++) {
            if (stringcmp(argv[i], "|") == 0) {
                  return i;
            }
      }
      return -1;
}

//muestra en pantalla el texto de la shell
static void shellText(t_shellData* shellData) {
      printStringWC(shellData->username, BLACK, LIGHT_BLUE);
      printStringWC(" $ > ", BLACK, LIGHT_BLUE);
}

//muestra la informacion recoletada sobre los registros obtenidos al haber presionado ctrl + s
void inforeg(int argc, char** args) {
      if (argc != 1) {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }
      uint64_t* regData = sys_inforeg();
      for (int i = 0; i < REGISTERS; i++) {
            printfBR(" > %s", regNames[i]);
            printHexWL(regData[i], 8);
            putchar('\n');
      }
      putchar('\n');
}

//cambia el nombre del usuario mostrado en la shell
void changeUsername(int argc, char** argv) {
      if (argc != 2) {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }
      cleanString(shell.username);
      strcpy(argv[1], shell.username);
}

//muestra la lista de comandos con sus descripciones
void help(int argc, char** args) {
      if (argc != 1) {
            printStringLn("Invalid ammount of arguments.");
            putchar('\n');
            return;
      }

      printStringLn("These shell commands are defined internally.  Type 'help' to see this list.");
      for (int i = 0; i < COMMANDS; i++) {
            printString(" >");
            printString(shell.commands[i].name);
            printString(": ");
            printStringLn(shell.commands[i].description);
      }
      putchar('\n');
}
