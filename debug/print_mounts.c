#include "../include/fs.h"


int print_mounts()
{
  int i = 0;
  MOUNT *mp = 0;

  printf("Known Mount Tabs for [%2d, %d]:\n", root->ino, root->dev);
  printf("Dev  # Inodes  # Blks  Bmap  Imap  Mount Point\n");
  printf("---  --------  ------  ----  ----  -----------\n");
  while(i < NMOUNT)
  {
    if (mounttab[i].busy == TRUE)
    {
      printf(" %d", mounttab[i].dev);
      printf("     %4d", mounttab[i].ninodes);
      printf("     %d", mounttab[i].nblocks);
      printf("     %d", mounttab[i].bmap);
      printf("     %d", mounttab[i].imap);
      printf("         %s\n", mounttab[i].mount_name);
    }

    i++;
  }
}