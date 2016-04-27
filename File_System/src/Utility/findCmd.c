#include "../include/fs.h"

// extern char *cmds[];

char *cmds[] = {"ls", "cd", "pwd", "mkdir", "rmdir", "creat", "rm", 
                "link", "symlink", "readlink", "stat", "chown", "chmod",
                "chgrp", "touch", "open", "close", "read", "write", 
                "lseek", "cat", "cp", "mv", "mount", "umount", "pfd", "pm", "pino",
                "quit", "clear", "menu", 0};

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