#include "../include/fs.h"

int _lseek ()
{
  int fd = -1;
  int position = -1;
  OFT *oftp = 0;

  if (!out[1])  // still need both arguments
  {
    printf("Missing file descriptor. (Usage: lseek fd position)\n");
    return -1;
  }
  if (!out[2])  // still need position argument
  {
    printf("Missing second argument. (Usage: lseek fd position)\n");
    return -2;
  }

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
      return -3;
    }

    if (fd >= NFD)
    {
      printf("\"%d\" : Not within specified range. (Valid: 0-%d)", fd, NFD - 1);
      return -4;
    }
  }
  if (strcmp(out[2], "0") == 0) // atoi cannot convert string "0" to int 0
  {
    position = 0;
  }
  else
  {
    position = atoi(out[2]);
    if (position == 0)
    {
      printf("\"%s\" : not an integer value.\n", out[2]);
      return -5;
    }
  }

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

  return prev_offset;
}