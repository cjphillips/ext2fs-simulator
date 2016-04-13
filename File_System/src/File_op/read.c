#include "../include/fs.h"

int _read ()
{
  int fd = -1, nbytes = -1;

  if (!out[1])
  {
    printf("Usage: read FD N_BYTES\n");
    return -1;
  }
  if (!out[2])
  {
    printf("Missing argument #2.\n");
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
      printf("\"%s\" : not an integer value. (Valid: 0,1,2,3...)\n", out[1]);
      return -2;
    }

    if (fd >= NFD)
    {
      printf("\"%d\" : Not within specified range. (Valid: 0-%d)\n", fd, NFD - 1);
      return -3;
    }
  }

  if (strcmp(out[2], "0") == 0) // atoi cannot convert string "0" to int 0
  {
    nbytes = 0;
  }
  else
  {
    nbytes = atoi(out[2]);
    if (nbytes == 0)
    {
      printf("\"%s\" : not an integer value. (Valid: 0,1,2,3...)\n", out[2]);
      return -2;
    }
  }

  char buf[BLKSIZE], *cp;
  int total_read = nbytes;
  int n = 0;

  while( (n = __read(fd, buf, nbytes)) > 0 )
  {
    total_read -= n;
    nbytes -= n;
    cp = buf;
    while(*cp)
    {
      if (*cp == '\n')
        printf("\n\r");
      else
        putchar(*cp);
      *cp++;
    }
    if (total_read == 0)
    {
      break;
    }

    bzero(buf, BLKSIZE);
  }

  if (n < 0)
  {
    return n;
  }

  return 0;
}