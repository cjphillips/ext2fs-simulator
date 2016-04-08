#include "include/fs.h"

char *Color_Begin_Prompt = "\033[1;31m";
char *Color_Begin_Proc = "\033[1;33m";
char *Color_End = "\033[0m";

char *cmds[] = {"ls", "cd", "pwd", "mkdir", "rmdir", "creat", 0};

int (*fptr[ ])() = {(int (*)()) ls, cd, pwd, _mkdir, _rmdir, _creat };

int main(int argc, char *argv[])
{
  int i, cmd;
  char line[MAX];

  if (argc > 1) {
    if (strcmp(argv[1], "-d") == 0)
      DEBUGGING = TRUE;
  }
  else
    DEBUGGING = FALSE;

  init(); /* IMPORTANT :: Initialize the filesystem */

  if(DEBUGGING) {
    printf("============DEBUG INFO============\n");
    printf("  Root Dev = %s\n", root_dev);
    printf("  Mounted Inode = %p\n", mp->mounted_inode);
    printf("  Mount point = %p\n", root->mount_ptr);
    

    printf("==================================\n");
  }

  printf("[%sP%d Running%s]\n", Color_Begin_Proc, running->pid, Color_End);

  while(TRUE) {
    out = 0;
    bzero(line, MAX);
    
    do {
      printf("%s[%scjp_sh%s]", Color_End, Color_Begin_Prompt, Color_End);
      if(running == &proc[0])
        printf("# ");
      else
	printf("$ ");
      fgets(line, MAX, stdin);
    } while(line[0] == '\n');
    line[strlen(line) - 1] = 0;

    numTokens = tokenize(line, " "); /* Places tokens into "out" above */

    int f_call = findCmd(out[0]);    // Search for a command
    int r = 0;
    
    /* For debug purposes */
    char cmd[256];
    bzero(cmd, 256);
    strcpy(cmd, out[0]);
    /* ------------------ */

    if (f_call < 0)
      printf("\"%s\" : Command not Found.\n", out[0]);
    else {
      r = fptr[f_call](); // Call the selected function
      if(DEBUGGING)
	printf("{(DEBUG) \"%s\" returned : %d}\n", cmd, r);
    }

    _free();

    if(DEBUGGING) {
      debug_dir(root);
      debug_dir(running->cwd);
    }
  }
}
