#include "../include/fs.h"

void iput(MINODE *mip)
{
  int blk, offset;
  char buf[BLKSIZE];
  INODE *ip;

  //if(mip == root) {
  //  if (DEBUGGING) 
  //    printf("iput ... putting root.\n");
  //  return;
  //}
  mip->ref_count--;

  if(DEBUGGING) {
    printf("..............DEBUG................\n");
    printf("iput ... mip(\"%s\") ref_count = %d\n", mip->name, mip->ref_count);
    printf("Place mip back? ... ");
  }
  if (mip->ref_count > 0) { // do not remove inode if still in use elsewhere
    if (DEBUGGING) {
      printf("No, referenced elsewhere.\n");
      printf("...................................\n");
    }
    return;
  }
  if (mip->dirty == FALSE) { // no need to write back
    if(DEBUGGING) {
      printf("No, not dirty.\n");
      printf("...................................\n");
    }
    return;
  }

  if(DEBUGGING) {
    printf("Yes.\n");
    printf("...................................\n");
  }
  
  blk = (mip->ino - 1) / INODES_PER_BLOCK + mp->iblock;
  offset = (mip->ino - 1) % INODES_PER_BLOCK;
  get_block(mip->dev, blk, buf);

  ip = (INODE *)buf + offset;
  memcpy(ip, &mip->Inode, sizeof(INODE));
  put_block(mip->dev, blk, buf); 
}