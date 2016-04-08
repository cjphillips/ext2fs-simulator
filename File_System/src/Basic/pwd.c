#include "../include/fs.h"

void rpwd(MINODE *at)
{
  char buf[BLKSIZE];
  int p_ino, dump;

  if (at == root) {
    return;
  }

  get_block(mp->dev, at->Inode.i_block[0], buf);
  findino(at, &dump, &p_ino);

  rpwd(iget(mp->dev, p_ino));
  printf("/%s", at->name);
}

int pwd()
{
  char buf[BLKSIZE];
  int p_ino;

  mip = running->cwd;

  if(mip == root) {
    printf("/\n");
    return 0;
  }

  rpwd(mip);
  printf("\n");

  //iput(mp->dev, p_ino);

  return 0;
}