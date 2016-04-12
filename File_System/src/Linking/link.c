#include "../include/fs.h"

int link()
{
  // TODO: Check that user is not attempting to link across devices.

  if (!out[1])
  {
    printf("Missing both arguements.n");
    return -1;
  }
  if (!out[2])
  {
    printf("Missing link name.\n");
    return -2;
  }

  int ino, n_ino_p, dev = running->cwd->dev;
  char path[INODE_NAME * 2], new_name[INODE_NAME];
  MINODE *mip, *n_pip;

  if(out[1][0] == '/')
  {
    dev = root->dev;
  }

  strcpy(new_name, basename(out[2]));
  strcpy(path, dirname (out[2]));

  ino = get_inode(out[1], &dev);
  if (ino < 0) // Inode not found
  {
    return ino;
  }

  n_ino_p = get_inode(path, &dev);
  if (n_ino_p < 0)
  {
    return n_ino_p;
  }

  n_pip = iget(dev, n_ino_p);

  if ((search (n_pip, new_name)) > 0)  // Check if the new link is already in the path
  {
    printf("\"%s\" : Already contains \"%s\".\n", path, new_name);
    iput(n_pip);
    return -3;
  }

  mip = iget(dev, ino);

  if(mip->Inode.i_mode == DIRECTORY)
  {
    printf("Cannot create a hard link to a directory.\n");
    iput(mip);
    iput(n_pip);
    return -3;
  }

  mip->Inode.i_links_count++;

  enter_name(n_pip, ino, new_name, mip->Inode.i_mode);

  n_pip->dirty = TRUE;
  mip->dirty = TRUE;
  iput(mip);
  iput(n_pip);

  return 0;
}