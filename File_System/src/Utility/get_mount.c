#include "../include/fs.h"

MOUNT *get_mount(int dev)
{
  int j = 0;

  while(j < NMOUNT)                            // Search for this device in the mount tab
  {
    if (mounttab[j].dev == dev)
    {
      return &mounttab[j];
    }

    j++;
  }

}