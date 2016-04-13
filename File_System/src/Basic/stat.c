#include "../include/fs.h"

int _stat()
{
  if (!out[1])
  {
    printf("Missing target file.\n");
    return -1;
  }

  int ino, dev = running->cwd->dev;
  char name[INODE_NAME];
  MINODE *mip;

  if(out[1][0] == '/')
  {
    dev = root->dev;
  }

  strncpy(name, basename(out[1]), INODE_NAME);

  ino = get_inode(out[1], &dev);
  if(ino < 0)
  {
    return ino;
  }

  mip = iget(dev, ino);

  printf("<----------------------------------------------------------STAT---->\n");
  printf("  Name: \"%s\"\n\n", name);
  printf("            Device:  %d\n", mip->dev);
  printf("           Inumber:  %d\n", mip->ino);
  printf("              Mode:  0x%x\n", mip->Inode.i_mode);
  printf("           # Links:  %d\n", mip->Inode.i_links_count);
  printf("               UID:  %d\n", mip->Inode.i_uid);
  printf("               GID:  %d\n", mip->Inode.i_gid);
  printf("              Size:  %d\n", mip->Inode.i_size);
  printf("        Block Size:  %d\n", BLKSIZE);
  printf("     # Data Blocks:  %d\n", mip->Inode.i_blocks);
  printf("       Last Access:  %s\n", ctime((time_t *)&mip->Inode.i_atime));
  printf("          Last Mod:  %s\n", ctime((time_t *)&mip->Inode.i_mtime));
  printf("   Last Status Chg:  %s\n", ctime((time_t *)&mip->Inode.i_ctime));
  printf("<------------------------------------------------------------------>\n");

  iput(mip);

  return 0;
}