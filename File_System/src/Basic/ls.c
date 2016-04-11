#include "../include/fs.h"

int ls()
{
  char buf[BLKSIZE];
  int i, ino, dev = running->cwd->dev;
  MINODE *at_dir = 0;


  if(!out[1] || strcmp(out[1], ".") == 0) // printing the current working directory
  {
    at_dir = iget(dev, running->cwd->ino);
  }
  else // Get the file path
  {
    if (out[1][0] == '/')
    {
      dev = root->dev;
    }

    if ((ino = get_inode(out[1], &dev)) < 0)
    {
      return ino;
    }

    at_dir = iget(dev, ino);
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
