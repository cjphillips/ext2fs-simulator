#include "../../include/fs.h"

int __read(int fd, char buf[], int nbytes)
{
  OFT *oftp = 0;
  int start = 0, remain= 0, avail = 0, lblk = 0, count = 0;
  char *cs = buf, *cp, readbuf[BLKSIZE];
  //MINODE *mip = 0;

  oftp = running->fd[fd];
  if (!oftp) // check that this fd is in use
  {
    printf("Descriptor #%d not in use.\n", fd);
    return -4;
  }

  if (oftp->mode != 0 && oftp->mode != 2) // Read OR Read-write
  {
    printf("File not open for reading.\n");
    return -5;
  }

  lblk   = oftp->offset / BLKSIZE;                         // Get the logical block to read from
  start  = oftp->offset % BLKSIZE;                         // Get the byte # to start reading at
  remain = BLKSIZE - start;                                // Find the remaining bytes on this block
  avail  = oftp->inode_ptr->Inode.i_size - oftp->offset;   // Find the available bytes left in the file

  int blk, *b_ptr = 0, *b_ptr2 = 0;
  char blk_buf[BLKSIZE], blk_buf2[BLKSIZE];

  while(nbytes && avail) // begin reading
  {
    if (lblk < 12) // direct blocks (1-12)
    {
      blk = oftp->inode_ptr->Inode.i_block[lblk];
    }
    else if (lblk >= 12 && lblk < 256 + 12)   // single indirect
    {
      if (b_ptr) // If the pointer has already been given a value, just increment
      {
        *b_ptr++;
      }
      else
      {
        get_block(oftp->inode_ptr->dev, oftp->inode_ptr->Inode.i_block[12], blk_buf);
        b_ptr = (int *)blk_buf;
      }
      blk = *b_ptr;
    }
    else                                      // double indirect 
    {
      if (b_ptr2) // already grabbed the double indirect block
      {
        if ((char *)b_ptr2 < blk_buf + BLKSIZE) // still within current block range
        {
          blk = *b_ptr2;
        }
        else // move first pointer to next block, have second pointer point to it 
        {
          *b_ptr++;
           get_block(oftp->inode_ptr->dev, *b_ptr, blk_buf2);
           b_ptr2 = (int *)blk_buf2;
        }
        lblk = *b_ptr2;
      }
      else // initial double indirect block access
      {
        get_block(oftp->inode_ptr->dev, oftp->inode_ptr->Inode.i_block[13], blk_buf);
        b_ptr = (int *)blk_buf;
        get_block(oftp->inode_ptr->dev, *b_ptr, blk_buf2);
        b_ptr2 = (int *)blk_buf2;
      }
    }

    get_block(oftp->inode_ptr->dev, blk, readbuf);  // Get the referenced data block
    cp = readbuf + start;

    while(remain > 0)
    {
      *cs++ = *cp++;
      oftp->offset++;
      count++;
      avail--;
      nbytes--;
      remain--;

      if(nbytes <= 0 || avail <= 0 || count == BLKSIZE)
      {
        goto finish;
      }
    }

    if ( (nbytes - BLKSIZE) <= 0 )
      remain = nbytes;
    else
      remain = BLKSIZE;

    lblk++;
  }

  finish:
  *cs = 0;

  if (DEBUGGING)
  {
    printf("Read %d byte(s) from fd #%d.\n", count, fd);
  }

  return count;
}