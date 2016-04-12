#include "../include/fs.h"

int _chmod ()
{
  // TODO : implement octal mapping.. 
  if (!out[1])
  {
    printf("Usage: chmod NEW_MODE TARGET\n");
    return -1;
  }
  if (!out[2])
  {
    printf("Missing target file.\n");
    return -2;
  }

  /*
  int new_mod = -1;

  if (out[1][0] == '-')
  {
    printf("Owner value must be a positive integer.\n");
    return -3;
  }

  if(strcmp(out[1], "0") == 0)  // Parse group value
  {
    new_mod = 0;
  }
  else
  {
    new_mod = atoi(out[1]);
    if(new_mod == 0)
    {
      printf("Invalid owner identifier (valid: 0+).\n");
      return -4;
    }
  }
  */

  int ino, dev = running->cwd->dev;
  char perm[20];
  MINODE *mip;

  strncpy(perm, out[1], 20);
  perm[19] = 9;

  ino = get_inode(out[2], &dev);
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

  if(perm[0] == '+' || perm[0] == '=')
  {
    if(perm[0] == '=') // Clear all bits first, then set
    {
      mip->Inode.i_mode &= ~(0777);
    }

    if(perm[1] == 'r') // set read
    {
      mip->Inode.i_mode |= 0444;
    }
    if(perm[1] == 'w') // set write
    {
      mip->Inode.i_mode |= 0222;
    }
    if(perm[1] == 'x') // set execute
    {
      mip->Inode.i_mode |= 0111;
    }

    mip->dirty = TRUE;
  }
  else if(perm[0] == '-')
  {
    if(perm[1] == 'r') // remove read
    {
      mip->Inode.i_mode &= ~(0444);
    }
    if(perm[1] == 'w') // remove write
    {
      mip->Inode.i_mode &= ~(0222);
    }
    if(perm[1] == 'x') // remove execute
    {
      mip->Inode.i_mode &= ~(0111);
    }

    mip->dirty = TRUE;
  }
  else
  {
    printf("Mode specified is not recognized.\n");
  }

  iput(mip);

  return 0;
}