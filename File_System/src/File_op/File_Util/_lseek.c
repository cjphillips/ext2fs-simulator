#include "../../include/fs.h"

int __lseek (int fd, int position)
{
  OFT *oftp = 0;

  oftp = running->fd[fd]; // Get the reference to the file descriptor
  if (!oftp)
  {
    printf("\"%d\" : not opened or in use.\n", fd);
    return -6;
  }
  if (oftp->offset < position)
  {
    printf("Provided position would move beyond current file bounds.\n");
    return -7;
  }
  int prev_offset = oftp->offset;
  oftp->offset = position;

  return 0;
}