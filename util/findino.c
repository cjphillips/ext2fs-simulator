#include "../include/fs.h"

int findino(MINODE *mip, int *ino, int *p_ino)
{
  char buf[BLKSIZE], *cp;
  DIR *ddp;
  int i;

  get_block(mip->dev, mip->Inode.i_block[0], buf);
  cp = buf;
  ddp = (DIR *)buf;

  *ino = ddp->inode; // current inode, 1st entry ALWAYS
  
  cp += ddp->rec_len;
  ddp = (DIR *)cp;

  *p_ino = ddp->inode; // parent's inode, 2nd entry ALWAYS

  return 0;
}