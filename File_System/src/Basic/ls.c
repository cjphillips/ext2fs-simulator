#include "../include/fs.h"

int ls()
{
  char buf[BLKSIZE];
  int i;
  MINODE *at_dir = 0;

  if(!out[1] || strcmp(out[1], ".") == 0) // printing the current working directory
  {
    at_dir = iget(mp->dev, running->cwd->ino);
  }
  else // Get the file path
  {
    int r = 0;
    if ((r = get_inode(&at_dir, out[1])) < 0)
    {
      return r;
    }
  }

  if (S_ISDIR(at_dir->Inode.i_mode)) { // printing directory
    print_dir(at_dir);
    iput(at_dir);
    return 0;
  }
  else {
    printf("\"%s\" : Not a directory.\n", out[1]);
  }

  iput(at_dir);
  
  return -1;
}
