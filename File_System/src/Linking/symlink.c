#include "../include/fs.h"

int symlink()
{
  if (!out[1])
  {
    printf("Usage: symlink TARGET LINK_NAME\n");
    return -1;
  }
  if (!out[2])
  {
    printf("Missing link name.\n");
    return -2;
  }
  if (strcmp(out[1], ".") == 0)
  {
    printf("Cannot link to \".\" (current directory).\n");
    return -3;
  }

  char old[60], new[INODE_NAME], buf[BLKSIZE], parent_path[INODE_NAME*2];              // The data block of the new link can only hold 60 bytes
  int n_ino, o_ino, p_ino, dev = running->cwd->dev;
  MINODE *mip_old, *mip_new, *new_pip;

  if(out[1][0] == '/')
  {
    dev = root->dev;
  }

  strncpy(old, out[1], 58);
  strcpy(new, out[2]);
  strcpy(parent_path, dirname(out[2]));
  old[59] = 0;

  o_ino = get_inode(old, &dev, FALSE);
  if(o_ino < 0)
  {
    return o_ino;
  }

  p_ino = get_inode(parent_path, &dev, FALSE);
  if (p_ino < 0)
  {
    return p_ino;
  }
  new_pip = iget(dev, p_ino);

  __creat(new_pip, new, LNK);
  iput(new_pip);

  n_ino = get_inode(new, &dev, FALSE);
  if(n_ino < 0)
  {
    return n_ino;
  }

  mip_new = iget(dev, n_ino);
  mip_old = iget(dev, o_ino);

  // TODO : check that devices are the same, cannot link over devices

  int bno = balloc();
  mip_new->Inode.i_block[0] = bno;
  mip_new->dirty = TRUE;

  mip_new->Inode.i_size = strlen(old);

  get_block(dev, bno, buf);
  strncpy(buf, old, 60);
  put_block(dev, bno, buf);

  iput(mip_new);
  iput(mip_old);

  return 0;
}