#ifndef SHELL_H
#define SHELL_H

#include <buffer.h>

#define COMMANDS 27
#define MAX_ARGS 10

typedef struct t_command t_command;
typedef struct t_shellData t_shellData;

typedef struct t_command {
      void (*command)(int, char **);
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