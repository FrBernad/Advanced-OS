#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <shell.h>

//prints commands man
void help(int argc, char** args, t_shellData * shellData);

//print registers, done in assembly
void inforeg(int argc,char ** args, t_shellData * shellData);

//recieves memory direction and prints 32 bytes after
void printmem(int argc,char ** args, t_shellData * shellData);

//displays system time
void time(int argc,char ** args, t_shellData * shellData);

//shows processor model and brand
void cpuInfo(int argc,char ** args, t_shellData * shellData);

//shows processor temp
void cpuTemp(int argc,char ** args, t_shellData * shellData);

//change username in shell
void changeUsername(int argc, char** argv, t_shellData * shellData);

//check zero exception
void checkZeroException(int argc,char ** args, t_shellData * shellData);

//check invalid op exception
void checkInvalidOpcodeException(int argc, char **args, t_shellData * shellData);

//shows arguments
void showArgs(int argc, char **args, t_shellData *shellData);

//list processes
void ps(int argc, char **args, t_shellData *shellData);

// Imprime su ID con un saludo cada una determinada cantidad de segundos.
void loop(int argc, char **args, t_shellData *shellData);

// Mata un proceso dado su ID.
void kill(int argc, char **args, t_shellData *shellData);

// Cambia la prioridad de un proceso dado su ID y la nueva prioridad.
void nice(int argc, char **args, t_shellData *shellData);

// Cambia el estado de un proceso entre bloqueado y listo dado su ID.
void block(int argc, char **args, t_shellData *shellData);

#endif