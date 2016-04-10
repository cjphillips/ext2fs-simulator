#include "../include/fs.h"

int ls()
{
  char buf[BLKSIZE];
  int i;

  if(!out[1]) // printing the current working directory
    mip = iget(mp->dev, running->cwd->ino);
  // TODO: get file path

  if (S_ISDIR(mip->Inode.i_mode)) { // printing directory
    print_dir(mip);
    iput(mip);
    return 0;
  }
  else {
    printf("\"%s\" : Not a directory.\n", out[1]);
  }

  iput(mip);
  
  return -1;
}
