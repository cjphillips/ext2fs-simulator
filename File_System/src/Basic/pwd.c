#include "../include/fs.h"

void rpwd(MINODE *at)
{
  char buf[BLKSIZE], temp[INODE_NAME];
  int p_ino, dump;

  if (at == root) {
    iput(at);
    return;
  }

  strcpy(temp, at->name);

  get_block(mp->dev, at->Inode.i_block[0], buf);
  findino(at, &dump, &p_ino);

  iput(at);
  rpwd(iget(mp->dev, p_ino));
  printf("/%s", temp);
}

int pwd()
{
  char buf[BLKSIZE];
  int p_ino;

  mip = iget(mp->dev, running->cwd->ino);

  if(mip == root) {
    printf("/\n");
    iput(mip);
    return 0;
  }

  rpwd(mip);
  printf("\n");

  //iput(mp->dev, p_ino);

  return 0;
}