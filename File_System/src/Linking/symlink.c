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
    printf("Cannot link to same directory.\n");
  }

  char old[60], new[INODE_NAME], buf[BLKSIZE];              // The data block of the new link can only hold 60 bytes
  int n_ino, o_ino, dev = running->cwd->dev;
  MINODE *mip_old, *mip_new;

  if(out[1][0] == '/')
  {
    dev = root->dev;
  }

  strncpy(old, out[1], 58);
  strcpy(new, out[2]);
  old[59] = 0;


  char arg[INODE_NAME];
  strcpy(arg, "creat ");
  arg[6] = 0;
  strcat(arg, new);
  tokenize(arg, " ");

  o_ino = get_inode(old, &dev);
  if(o_ino < 0)
  {
    return o_ino;
  }

  if ((_creat()) < 0)                         // Create the new file
  {
    return -3;
  }
  n_ino = get_inode(new, &dev);


  if(n_ino < 0)
  {
    return n_ino;
  }

  mip_new = iget(dev, n_ino);
  mip_old = iget(dev, o_ino);

  // TODO : check that devices are the same, cannot link over devices

  mip_new->Inode.i_mode = LNK;
  mip_new->dirty = TRUE;

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