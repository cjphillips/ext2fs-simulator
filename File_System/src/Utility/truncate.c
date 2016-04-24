#include "../include/fs.h"

void _truncate (MINODE *mip)
{
  if (mip->Inode.i_mode != REG_FILE)
  {
    return;
  }

  char buf[BLKSIZE];
  int i = 0, *bptr, *bptr_2;

  while(i < 12 && mip->Inode.i_block[i]) // remove first 12 direct blocks
  {
    bdealloc(mip->Inode.i_block[i]);
    mip->Inode.i_block[i] = 0;
    i++;
  }

  if(mip->Inode.i_block[i])              // Must remove single-indirect blocks
  {
    get_block(mip->dev, mip->Inode.i_block[i], buf);
    bptr = (int *)buf;

    while(*bptr && (char *)bptr < buf + BLKSIZE)
    {
      bdealloc(*bptr);
      *bptr = 0;
      *bptr++;
    }
    bdealloc(mip->Inode.i_block[i]); // Remove the block of pointers itself

    mip->Inode.i_block[i] = 0;
    i++;
  }
  if(mip->Inode.i_block[i])              // Must remove double-indirect blocks
  {
    char buf_2[BLKSIZE];

    get_block(mip->dev, mip->Inode.i_block[i], buf);
    bptr = (int *)buf;

    while(*bptr && (char *)bptr < buf + BLKSIZE) // Grap all block pointers (256 at most).
    {
      get_block(mip->dev, *bptr, buf_2);         
      bptr_2 = (int *)buf;

      while(bptr_2 && (char *)bptr_2 < buf_2 + BLKSIZE) // deallocate each block that this pointer (bptr) points to 
      {
        bdealloc(*bptr_2);
        *bptr_2 = 0;
        *bptr_2++;
      }
      bdealloc(*bptr);
      *bptr = 0;
      *bptr++;
    }
    bdealloc(mip->Inode.i_block[i]);

     mip->Inode.i_block[i] = 0;
    i++;
  }
  // TODO : Triple indirect blocks... not necessary at this point, however.

  mip->Inode.i_atime = time(0L); // update time field
  mip->Inode.i_size = 0;         // not data blocks == no size
  mip->dirty = TRUE;
}