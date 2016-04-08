#include "../include/fs.h"

int ls()
{
  char buf[BLKSIZE];
  int i;

  if(!out[1]) // printing the current working directory
    mip = running->cwd;
  // TODO: get file path

  if (S_ISDIR(mip->Inode.i_mode)) { // printing directory
    print_dir(mip);
    return 0;
  }
  else {
    printf("\"%s\" : Not a directory.\n", out[1]);
  }
  
  return -1;
}
