#include "../../include/fs.h"

int __write(int fd, char buf[], int nbytes)
{
  OFT *oftp = 0;
  int start = 0, remain = 0, lblk = 0, blk = 0;
  char writebuf[BLKSIZE], *cs, *cp = buf;

  oftp = running->fd[fd];
  if (!oftp) // check that this fd is in use
  {
    printf("Descriptor #%d not in use.\n", fd);
    return -4;
  }

  if (oftp->mode == 0) // Cannot be in read mode
  {
    printf("File not open for writing.\n");
    return -5;
  }


  while (nbytes > 0)
  {
    lblk   = oftp->offset / BLKSIZE;                         // Get the logical block to read from
    start  = oftp->offset % BLKSIZE;                         // Get the byte # to start reading at
   
    if (lblk < 12) // direct blocks (1-12)
    {
      if (oftp->inode_ptr->Inode.i_block[lblk] = 0)
      {
        blk = oftp->inode_ptr->Inode.i_block[lblk] = balloc(mip->dev);
        oftp->inode_ptr->dirty = TRUE;
      }
      else
      {
        blk = oftp->inode_ptr->Inode.i_block[lblk];
      }
    }
    else if (lblk >= 12 && lblk < 256 + 12)   // single indirect
    {
    }
    else                                      // double indirect 
    {
    }

    get_block(oftp->inode_ptr->dev, blk, writebuf);
    cs = writebuf + start;
    remain = BLKSIZE - start;

    while(remain > 0)
    {
      *cs++ = *cp++;
      remain--;
      nbytes--;
      oftp->offset++;
      if (oftp->offset > oftp->inode_ptr->Inode.i_size)
      {
        oftp->inode_ptr->Inode.i_size++;
      }
      if (nbytes < 0)
      {
        goto finish;
      }
    }

    put_block(oftp->inode_ptr->dev, blk, writebuf); 
    lblk++;
  }

  finish:
  *cs = 0;
  put_block(oftp->inode_ptr->dev, blk, writebuf);


}