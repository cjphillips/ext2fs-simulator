#include "../include/fs.h"

int readlink()
{
  if (!out[1])
  {
    printf("Missin link name.\n");
    return -1;
  }


  int ino, dev = running->cwd->dev;
  char buf[BLKSIZE], name[INODE_NAME];
  MINODE *mip;

  strncpy(name, basename(out[1]), INODE_NAME);

  if(out[1][0] == '/')
  {
    dev = root->dev;
  }

  ino = get_inode(out[1], &dev, FALSE);   // Get ino from path
  mip = iget(dev, ino);            // Get inode into memory

  if (mip->Inode.i_mode != LNK)
  {
    printf("\"%s\" : Not a symbolic link.\n", name);
    iput(mip);
    return -2;
  }

  get_block(dev, mip->Inode.i_block[0], buf); // Get the symbolic link's first data block (where the target name is stored).

  printf("%s\n", buf);

  iput(mip);

  return 0;
}