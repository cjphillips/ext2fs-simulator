#include "../include/fs.h"

int touch ()
{
  if(!out[1])
  {
    printf("Missing target file.\n");
    return -1;
  }

  int ino, dev = running->cwd->dev;
  char name[INODE_NAME], arg[INODE_NAME], path[INODE_NAME*2];
  MINODE *mip;

  strncpy(name, basename(out[1]), INODE_NAME);
  strncpy(path, out[1], INODE_NAME*2);

  ino = get_inode(out[1], &dev, FALSE);
  if (ino < 0) // If no file is present, create it
  {
    return ino;
  }

  mip = iget(ino, dev);

  mip->Inode.i_atime = mip->Inode.i_mtime = time(0L);
  mip->dirty = TRUE;

  iput(mip);

  return 0;
}