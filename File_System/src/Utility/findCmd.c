#include "../include/fs.h"

extern char *cmds[];

int findCmd(char *command)
{
  int i = 0;

  for(i = 0; cmds[i] != 0; i++)
  {
    if (strcmp(command, cmds[i]) == 0) {
     //printf("Command: \'%s\'\n", cmds[i]);
      return i;
    }
  }

  return -1;
}