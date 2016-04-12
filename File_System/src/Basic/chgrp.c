#include "../include/fs.h"

int _chgrp ()
{
  if (!out[1])
  {
    printf("Usage: chgrp NEW_GROUP TARGET\n");
    return -1;
  }
  if (!out[2])
  {
    printf("Missing target file.\n");
    return -2;
  }

  int new_group = -1;

  if (out[1][0] == '-')
  {
    printf("Group value must be a positive integer.\n");
    return -3;
  }
  if(strcmp(out[1], "0") == 0)  // Parse group value
  {
    new_group = 0;
  }
  else
  {
    new_group = atoi(out[1]);
    if(new_group == 0)
    {
      printf("Invalid group identifier (valid: 0+).\n");
      return -4;
    }
  }

  int ino, dev = running->cwd->dev;
  MINODE *mip;

  ino = get_inode(out[2], &dev);
  if(ino < 0)
  {
    return ino;
  }

  mip = iget(dev, ino);

  if (running->gid != 0 && running->gid != mip->Inode.i_gid)
  {
    printf("Permission denied.\n");
    iput(mip);
    return -5;
  }

  mip->Inode.i_gid = new_group;
  mip->dirty = TRUE;

  iput(mip);

  return 0;
}