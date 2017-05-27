#include "../include/fs.h"

void put_block(int fd, int blk, char buf[])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  write(fd, buf, BLKSIZE);
}