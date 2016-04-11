#include "../include/fs.h"

int _close ()
{
  if (!out[1])
  {
    printf("No file descriptor provided.\n");
    return -1;
  }

  int fd = -1;
  OFT *oftp = 0;
  MINODE *mip;

  if (strcmp(out[1], "0") == 0) // atoi cannot convert string "0" to int 0
  {
    fd = 0;
  }
  else
  {
    fd = atoi(out[1]);
    if (fd == 0)
    {
      printf("\"%s\" : not an integer value. (Valid: 0-%d)\n", out[1], NFD - 1);
      return -2;
    }

    if (fd >= NFD)
    {
      printf("\"%d\" : Not within specified range. (Valid: 0-%d)", fd, NFD - 1);
      return -3;
    }
  }

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
    return 0;
  }

  mip = oftp->inode_ptr;
  iput(mip);

  return 0;
}