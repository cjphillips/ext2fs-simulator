#include "../include/fs.h"

void get_block(int fd, int blk, char buf[])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  read(fd, buf, BLKSIZE);
}