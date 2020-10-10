#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <shell.h>

//prints commands man
void help(int argc, char** args, uint8_t fg);

//print registers, done in assembly
void inforeg(int argc,char ** args, uint8_t fg);

//recieves memory direction and prints 32 bytes after
void printmem(int argc,char ** args, uint8_t fg);

//displays system time
void time(int argc,char ** args, uint8_t fg);

//shows processor model and brand
void cpuInfo(int argc,char ** args, uint8_t fg);

//shows processor temp
void cpuTemp(int argc,char ** args, uint8_t fg);

//change username in shell
void changeUsername(int argc, char** argv, uint8_t fg);

//check zero exception
void checkZeroException(int argc,char ** args, uint8_t fg);

//check invalid op exception
void checkInvalidOpcodeException(int argc, char **args, uint8_t fg);

//shows arguments
void showArgs(int argc, char **args, uint8_t fg);

//list processes
void ps(int argc, char **args, uint8_t fg);

// Imprime su ID con un saludo cada una determinada cantidad de segundos.
void loop(int argc, char **args, uint8_t fg);

// Mata un proceso dado su ID.
void kill(int argc, char **args, uint8_t fg);

// Cambia la prioridad de un proceso dado su ID y la nueva prioridad.
void nice(int argc, char **args, uint8_t fg);

// Cambia el estado de un proceso entre bloqueado y listo dado su ID.
void block(int argc, char **args, uint8_t fg);

// Memory manager test
void testMM(int argc, char **args, uint8_t fg);

// Processes test
void testProcesses(int argc, char **args, uint8_t fg);

// Priorities test
void testPriority(int argc, char **args, uint8_t fg);

// Priorities test
void testSync(int argc, char **args, uint8_t fg);

// Priorities test
void testNoSync(int argc, char **args, uint8_t fg);

#endif