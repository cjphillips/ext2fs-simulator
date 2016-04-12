#include "../include/fs.h"

int link()
{
  if (!out[1])
  {
    printf("Missing both arguements.n");
    return -1;
  }
  if (!out[2])
  {
    printf("Missing link name.\n");
    return -2;
  }

  int ino, dev = running->cwd->dev;
  MINODE *mip;

  if(out[1][0] == '/')
  {
    dev = root->dev;
  }

  ino = get_inode(out[1], &dev);

  if (ino < 0) // Inode not found
  {
    return ino;
  }

  mip = iget(dev, ino);

  if(mip->Inode.i_mode == DIRECTORY)
  {
    printf("Cannot create a link to a directory.\n");
    iput(mip);
    return -3;
  }

  return 0;
}