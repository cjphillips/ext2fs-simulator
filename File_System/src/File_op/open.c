#include "../include/fs.h"

int _open ()
{
  int mode = 0, dev, ino;
  MINODE *mip;

  if (!out[1])
  {
    printf("No target file specified.\n");
    return -1;
  }
  if (out[2]) // mode has been specified - default is 0 (RD)
  {
    switch(atoi(out[2]))
    {
      case 0: break;
      case 1: mode = 1; break;
      case 2: mode = 2; break;
      case 3: mode = 3; break;
      default:  
        printf ("Invalid mode. (Valid: 0-3)\n");
        return -2;
    }
  }

  if (out[1][0] == '/')  // Get the dev to open from
  {
    dev = root->dev;
  }
  else
  {
    dev = running->cwd->dev;
  }

  //ino = get_inode(mip, &dev);

  return 0;
}