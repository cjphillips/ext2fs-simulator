#include "../include/fs.h"

void rpwd(MINODE *at, int *dev)
{
  char buf[BLKSIZE], temp[INODE_NAME];
  int p_ino, dump, j = 0;

  if (at == root) {
    iput(at);
    return;
  }
  if (at->ino == 2 && at->dev != root->dev) // Crossing a mount point
  {
    MINODE *mount_mip = 0;
    while(j < NMOUNT)                            // Search for this device in the mount tab
    {
      if (mounttab[j].dev == at->dev)
      {
        mount_mip = mounttab[j].mounted_inode;
        break;
      }

      j++;
    }

    *dev = mount_mip->dev;                    // Change to this inodes device

    iput(at);
    at = iget(*dev, mount_mip->ino);
  }

  strcpy(temp, at->name);

  get_block(*dev, at->Inode.i_block[0], buf);
  findino(at, &dump, &p_ino);

  iput(at);
  at = iget(*dev, p_ino);

  rpwd(at, dev);
  printf("/%s", temp);
}

int pwd()
{
  char buf[BLKSIZE];
  int p_ino, dev;

  mip = iget(running->cwd->dev, running->cwd->ino);
  dev = mip->dev;

  if(mip == root) {
    printf("/\n");
    iput(mip);
    return 0;
  }

  rpwd(mip, &dev);
  printf("\n");

  //iput(mp->dev, p_ino);

  return 0;
}