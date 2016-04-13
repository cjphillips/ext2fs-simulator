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
      printf("\"%d\" : Not within specified range. (Valid: 0-%d)\n", fd, NFD - 1);
      return -3;
    }
  }

  return __close(fd);
}