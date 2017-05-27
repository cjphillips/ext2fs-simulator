#include "../include/fs.h"

int ls()
{
  char buf[BLKSIZE], name[INODE_NAME];
  int i, ino, dev = running->cwd->dev;
  MINODE *at_dir = 0;


  if(!out[1] || strcmp(out[1], ".") == 0) // printing the current working directory
  {
    at_dir = iget(dev, running->cwd->ino);
    if (at_dir->mounted == TRUE) // At the root inode of another device
    {
      dev = at_dir->mount_ptr->dev;
      iput(at_dir);
      at_dir = iget(dev, 2);
    }
  }
  else // Get the file path
  {
    if (out[1][0] == '/')
    {
      dev = root->dev;
    }
    strncpy(name, basename(out[1]), INODE_NAME);

    if ((ino = get_inode(out[1], &dev, FALSE)) < 0)
    {
      return ino;
    }

    at_dir = iget(dev, ino);
  }

  if (S_ISDIR(at_dir->Inode.i_mode)) { // printing directory
    print_dir(at_dir, dev);
    iput(at_dir);
    return 0;
  }
  else {
    printf("\"%s\" : Not a directory.\n", name);
  }

  iput(at_dir);
  
  return -1;
}
