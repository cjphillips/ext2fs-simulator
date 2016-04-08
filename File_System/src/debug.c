#include "include/fs.h"

int debug_dir(MINODE *ip)
{
  char buf[BLKSIZE], *cp, temp[256];
  DIR *dptr;
  int i = 0;

  printf("~~~~~~~~~~DEBUG :: DIR~~~~~~~~~~~\n");
  printf("  -> Directory \"%s\"\n\n", ip->name);
  printf(" ino   rec_len   name_len   name\n");
  while(i < 12 && ip->Inode.i_block[i]) {
    get_block(mp->dev, ip->Inode.i_block[i], buf);
    cp = buf;
    dptr = (DIR *)buf;

    while(cp < buf + BLKSIZE) {
      printf("  %2d    ", dptr->inode);
      printf("%4d      ", dptr->rec_len);
      printf("%3d     ", dptr->name_len);
      strncpy(temp, dptr->name, dptr->name_len);
      temp[dptr->name_len] = 0;
      printf("%s\n", temp);

      cp += dptr->rec_len;
      dptr = (DIR *)cp;
    }

    i++;
  }
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
