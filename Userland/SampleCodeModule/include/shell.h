#ifndef SHELL_H
#define SHELL_H

#include <buffer.h>

#define COMMANDS 20
#define MAX_ARGS 4

typedef struct t_command t_command;
typedef struct t_shellData t_shellData;

typedef struct t_command {
      void (*command)(int, char **, uint8_t);
      char *name;
      char *description;
} t_command;

typedef struct t_shellData {
      t_command commands[COMMANDS];
      t_buffer buffer;
      char username[BUFFER_SIZE];
} t_shellData;

void runShell(int argc, char *argv[]);

#endif