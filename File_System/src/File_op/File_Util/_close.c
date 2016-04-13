#include "../../include/fs.h"

int __close (int fd)
{
  OFT *oftp = 0;
  MINODE *mip;

  oftp = running->fd[fd]; // Get the reference to the file descriptor
  if (!oftp)
  {
    printf("\"%d\" : not opened or in use.\n", fd);
    return -4;
  }

  running->fd[fd] = 0;    // Close the reference in the running process
  oftp->ref_count--;      // Remove a reference count (still may not be zero for READ-ONLY cases)

  if(oftp->ref_count > 0) // Still open elsewhere
  {
    return -5;
  }

  mip = oftp->inode_ptr;
  iput(mip);

  return 0;
}