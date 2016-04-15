#include "../../include/fs.h"

int __write(int fd, char buf[], int nbytes)
{
  OFT *oftp = 0;
  int start = 0, remain = 0, lblk = 0, blk = 0;
  char writebuf[BLKSIZE], tempbuf[BLKSIZE], blkbuf[BLKSIZE], blkbuf2[BLKSIZE], *cs, *cp = buf;

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

  int *b_ptr = 0, *b_ptr2 = 0;

  while (nbytes > 0)
  {
    lblk   = oftp->offset / BLKSIZE;                         // Get the logical block to read from
    start  = oftp->offset % BLKSIZE;                         // Get the byte # to start reading at
   
    if (lblk < 12) // direct blocks (1-12)
    {
      if (oftp->inode_ptr->Inode.i_block[lblk] == 0)
      {
        blk = oftp->inode_ptr->Inode.i_block[lblk] = balloc(oftp->inode_ptr->dev);
        if (oftp->inode_ptr->Inode.i_block[lblk] == 0)
        {
          printf("Could not finish write. Not enough space.\n");
          return -1;
        }
        oftp->inode_ptr->dirty = TRUE;

        get_block(oftp->inode_ptr->dev, blk, tempbuf);
        bzero(tempbuf, BLKSIZE); // ensure that no contents are on the block
        put_block(oftp->inode_ptr->dev, blk, tempbuf);
      }
      else
      {
        blk = oftp->inode_ptr->Inode.i_block[lblk];
      }
    }
    else if (lblk >= 12 && lblk < 256 + 12)   // single indirect
    {
      if (oftp->inode_ptr->Inode.i_block[12] == 0)
      {
        oftp->inode_ptr->Inode.i_block[12] = balloc(oftp->inode_ptr->dev);
        if (oftp->inode_ptr->Inode.i_block[12] == 0)
        {
          printf("Could not finish write. Not enough space.\n");
          return -2;
        }
        oftp->inode_ptr->dirty = TRUE;

        get_block(oftp->inode_ptr->dev, oftp->inode_ptr->Inode.i_block[12], tempbuf);
        bzero(tempbuf, BLKSIZE);
        put_block(oftp->inode_ptr->dev, oftp->inode_ptr->Inode.i_block[12], tempbuf);
        oftp->inode_ptr->Inode.i_blocks += 2;
      }
      if (b_ptr)
      {
        *b_ptr++;
      }
      else
      {
        get_block(oftp->inode_ptr->dev, oftp->inode_ptr->Inode.i_block[12], blkbuf);
        b_ptr = (int *)blkbuf;
      }

      if (*b_ptr == 0) // must allocate a new block first
      {
        *b_ptr = balloc(oftp->inode_ptr->dev);
        if (*b_ptr == 0)
        {
          printf("Could not finish write. Not enough space.\n");
          return -3;
        }
        oftp->inode_ptr->dirty = TRUE;

        get_block(oftp->inode_ptr->dev, *b_ptr, tempbuf);
        bzero(tempbuf, BLKSIZE);
        put_block(oftp->inode_ptr->dev, *b_ptr, tempbuf);
        oftp->inode_ptr->Inode.i_blocks += 2;
      }

      blk = *b_ptr;
    }
    else                                      // double indirect 
    {
      if (b_ptr2) // already grabbed the double indirect block
      {
        if ((char *)b_ptr2 < blkbuf + BLKSIZE) // still within current block range
        {
          blk = *b_ptr2;
        }
        else // move first pointer to next block, have second pointer point to it 
        {
          *b_ptr++;
           get_block(oftp->inode_ptr->dev, *b_ptr, blkbuf2);
           b_ptr2 = (int *)blkbuf2;
        }
        lblk = *b_ptr2;
      }
      else // initial double indirect block access
      {
        get_block(oftp->inode_ptr->dev, oftp->inode_ptr->Inode.i_block[13], blkbuf);
        b_ptr = (int *)blkbuf;
        get_block(oftp->inode_ptr->dev, *b_ptr, blkbuf2);
        b_ptr2 = (int *)blkbuf2;
      }

      if (*b_ptr2 == 0) // must allocate a new block first
      {
        *b_ptr2 = balloc(oftp->inode_ptr->dev);
        oftp->inode_ptr->dirty = TRUE;

        get_block(oftp->inode_ptr->dev, *b_ptr2, tempbuf);
        bzero(tempbuf, BLKSIZE);
        put_block(oftp->inode_ptr->dev, *b_ptr2, tempbuf);
      }

      blk = *b_ptr2;
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
      if (nbytes <= 0)
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

  return 0;
}