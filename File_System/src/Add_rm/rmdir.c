#include "../include/fs.h"

void __rmdir  (MINODE *pip, char *name);

int _rmdir()
{
  if(numTokens < 2) { // no name provided
    printf("No target provided.\n");
    return -1;
  }
  if(strcmp(out[1], "/") == 0) { // cannot make root
    printf("\"/\" :  Cannot remove.\n");
    return -4;
  }

  int dev;
  int p_ino;
  MINODE *pip;
  char buf[BLKSIZE];
  
  char base[INODE_NAME], dirs[INODE_NAME*6];

  strcpy(base, basename(out[1])); // the target name
  strcpy(dirs, dirname (out[1])); // the parent path
  dirs[strlen(dirs)] = 0;

  if (DEBUGGING)
    printf("Path : %s\nBase : %s\n", dirs, base);

  // get the parent's inode, starting from the root OR the cwd
  int r;
  if((r = get_inode(&pip, dirs)) < 0) {
    return r;
  }

  return 0;
}

