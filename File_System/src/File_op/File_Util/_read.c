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

  int blk;

  while(nbytes && avail) // begin reading
  {
    if (lblk < 12) // direct blocks (1-12)
    {
      blk = oftp->inode_ptr->Inode.i_block[lblk];
    }
    else if (lblk >= 12 && lblk < 256 + 12)   // single indirect
    {
    }
    else                                      // double indirect 
    {
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