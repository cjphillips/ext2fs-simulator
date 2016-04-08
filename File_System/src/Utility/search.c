#include "../include/fs.h"

int search(MINODE *ip, char *name)
{
  char buf[BLKSIZE], *cp;
  char temp[EXT2_NAME_LEN];
  int i;

  for(i = 0; i < 12 && ip->Inode.i_block[i]; i++) {
    get_block(ip->dev, ip->Inode.i_block[i], buf);
    cp = buf;
    dp = (DIR *)buf;
    while(cp < buf + BLKSIZE) {
      bzero(temp, EXT2_NAME_LEN);
      strncpy(temp, dp->name, dp->name_len);
    
      if(strcmp(temp, name) == 0)
  return dp->inode;

      cp += dp->rec_len;
      dp = (DIR *)cp;
    }
  }

  return -1;
}