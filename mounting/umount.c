#include "../include/fs.h"

int umount ()
{
  int i = 0;
  MOUNT *mp = 0;
  MINODE *mip = 0;

  if (!out[1])
  {
    printf("Missing target.\n");
    return -1;
  }

  while (i < NMOUNT) // check to see if the passed filesystem is actually mounted
  {
    if (strcmp(mounttab[i].name, out[1]) == 0)
    {
      mp = &mounttab[i];
    }

    i++;
  }
  if (!mp)
  {
    printf("\"%s\" : not mounted.\n", out[1]);
    return -2;
  }

  i = 0;
  while (i < NMINODES) // ensure that the mount is not in use
  {
    if (minode[i].ref_count > 0 && minode[i].dev == mp->dev)
    {
      printf("Mount still active.\n");
      return -2;
    }

    i++;
  }

  /* Reset mount point's mounting info */
  mip = mp->mounted_inode;
  mp->mounted_inode = 0;
  mp->busy = FALSE;

  mip->mounted = FALSE;
  iput(mip);

  return 0;
}