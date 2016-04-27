#include "../include/fs.h"

int findname(INODE *parent, int ino_at, char name[], int dev)
{
  char buf[BLKSIZE], *cp, temp[256];
  DIR *dp;
  int i, ino;

  for(i = 0; i < 12 && parent->i_block[i]; i++) { // search all twelve direct blocks
    get_block(dev, parent->i_block[i], buf);
    cp = buf;
    dp = (DIR *)buf;
    while(cp < buf + BLKSIZE) {
      if (ino_at == dp->inode && dp->name[0] != '.') {
        strncpy(name, dp->name, dp->name_len);
        name[dp->name_len] = 0;
        return 0;
      }

      cp += dp->rec_len;
      dp = (DIR *)cp;
    }
  }

  return -1;
}