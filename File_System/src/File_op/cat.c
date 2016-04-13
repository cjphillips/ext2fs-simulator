#include "../include/fs.h"

int cat ()
{
  if (!out[1])
  {
    printf("Missing file name.\n");
    return -1;
  }

  char buf[BLKSIZE], path[INODE_NAME*2], dummy = 0;
  int n = -1, fd = -1;
  MINODE *mip = 0;

  strcpy(path, out[1]);

  fd = __open(path, 0); // Open the file for read
  if (fd < 0)
  {
    return fd;
  }

  char *cp;
  while ((n = __read(fd, buf, BLKSIZE)) > 0)
  {
    buf[n] = 0;
    cp = buf;
    while (*cp)
    {
      if (*cp == '\n')
        printf("\n\r");
      else
        putchar(*cp);

      *cp++;
    }
    bzero(buf, BLKSIZE);
  }

  __close(fd);

  if (n < 0)
  {
    return n;
  }

  return 0;
}