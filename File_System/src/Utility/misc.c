#include "../include/fs.h"

void decFree(bool inode)
{
  char buf[BLKSIZE];

  /* decrement super block free inode count */
  get_block(mp->dev, 1, buf);
  sp = (SUPER *)buf;
  if(inode)
    sp->s_free_inodes_count--;
  else
    sp->s_free_blocks_count--;
  put_block(mp->dev, 1, buf);

  int gp_blk = sp->s_first_data_block + 1;

  /* decrement group desc free inode count */
  get_block(mp->dev, 2, buf);
  gp = (GD *)buf;
  if(inode)
    gp->bg_free_inodes_count--;
  else
    gp->bg_free_blocks_count--;
  put_block(mp->dev, gp_blk, buf);
}

void _free()
{
  int j = 0;

  if (out) { // free out
    while(j < numTokens) {
      free(out[j]);
      j++;
    }
    free(out);
  }
}