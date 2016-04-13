#include "../include/fs.h"

int _lseek ()
{
  int fd = -1;
  int position = -1;

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

  return __lseek(fd, position);
}