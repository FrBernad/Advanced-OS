#ifndef PIPES_H
#define PIPES_H

int initPipes();
int popen(char* pipeName);
int closePipe(int pipeIndex);
int writePipe(int pipeIndex, char* string);
int readPipe(int pipeIndex);
int writeCharPipe(int pipeIndex, char c);
void dumpPipes();

#endif