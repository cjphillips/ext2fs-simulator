#include "../include/fs.h"

int _chown ()
{
  if (!out[1])
  {
    printf("Usage: chown NEW_OWNER TARGET\n");
    return -1;
  }
  if (!out[2])
  {
    printf("Missing target file.\n");
    return -2;
  }

  int new_own = -1;

  if (out[1][0] == '-')
  {
    printf("Owner value must be a positive integer.\n");
    return -3;
  }
  if(strcmp(out[1], "0") == 0)  // Parse group value
  {
    new_own = 0;
  }
  else
  {
    new_own = atoi(out[1]);
    if(new_own == 0)
    {
      printf("Invalid owner identifier (valid: 0+).\n");
      return -4;
    }
  }

  int ino, dev = running->cwd->dev;
  MINODE *mip;

  ino = get_inode(out[2], &dev, FALSE);
  if(ino < 0)
  {
    return ino;
  }

  mip = iget(dev, ino);

  if (running->uid != 0 && running->uid != mip->Inode.i_uid)
  {
    printf("Permission denied.\n");
    iput(mip);
    return -5;
  }

  mip->Inode.i_uid = new_own;
  mip->dirty = TRUE;

  iput(mip);

  return 0;
}