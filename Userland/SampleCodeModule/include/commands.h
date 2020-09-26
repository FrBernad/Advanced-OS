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
void showArgs(int argc, char **args, t_shellData * shellData);

#endif