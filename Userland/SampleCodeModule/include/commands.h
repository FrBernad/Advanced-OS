#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <shell.h>

//prints commands man
void help(int argc, char** args);

//print registers, done in assembly
void inforeg(int argc,char ** args);

//recieves memory direction and prints 32 bytes after
void printmem(int argc,char ** args);

//displays system time
void time(int argc,char ** args);

//shows processor model and brand
void cpuInfo(int argc,char ** args);

//shows processor temp
void cpuTemp(int argc,char ** args);

//change username in shell
void changeUsername(int argc, char** argv);

//check zero exception
void checkZeroException(int argc,char ** args);

//check invalid op exception
void checkInvalidOpcodeException(int argc, char **args);

//shows arguments
void showArgs(int argc, char **args);

//list processes
void ps(int argc, char **args);

// Imprime su ID con un saludo cada una determinada cantidad de segundos.
void loop(int argc, char **args);

// Mata un proceso dado su ID.
void kill(int argc, char **args);

// Cambia la prioridad de un proceso dado su ID y la nueva prioridad.
void nice(int argc, char **args);

// Cambia el estado de un proceso a bloqueado.
void block(int argc, char **args);

// Cambia el estado de un proceso a listo dado su ID.
void unblock(int argc, char **args);

// Memory manager test
void testMM(int argc, char **args);

// Processes test
void testProcesses(int argc, char **args);

// Priorities test
void testPriority(int argc, char **args);

// Priorities test
void testSync(int argc, char **args);

// Priorities test
void testNoSync(int argc, char **args);

// dump sempahores
void dumpSemaphores(int argc, char** args);

// dump pipes
void dumpPipes(int argc, char **args);

// prints characters from input
void cat(int argc, char** args);

// counts the lines recieved from input
void wc(int argc, char **args);

// filters the vocals recieved from input
void filter(int argc, char **args);

// makes a dump of the memory manager state
void dumpMM(int argc, char **args);

#endif