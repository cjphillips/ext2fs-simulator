#include "../include/fs.h"

void __unlink (MINODE *toRemove, MINODE *pip, char *name);

int _unlink()
{
  if(numTokens < 2) { // no name provided
    printf("No target provided.\n");
    return -1;
  }
  if(strcmp(out[1], "/") == 0) { // cannot make root
    printf("\"/\" :  Cannot remove.\n");
    return -4;
  }

  int dev = running->cwd->dev;
  int ino;
  MINODE *pip;
  char buf[BLKSIZE];

  if(out[1][0] == '/')
  {
    dev = root->dev;
  }
  
  char base[INODE_NAME], dirs[INODE_NAME*6];

  strcpy(base, basename(out[1])); // the target name
  strcpy(dirs, dirname (out[1])); // the parent path
  dirs[strlen(dirs)] = 0;

  if (DEBUGGING)
    printf("Path : %s\nBase : %s\n", dirs, base);

  // get the parent's inode, starting from the root OR the cwd
  if ((ino = get_inode(dirs, &dev, FALSE)) < 0) {
    return ino;
  }

  pip = iget(dev, ino);

  ino = search(pip, base);
  if (ino < 1) { // inode not found
    printf("\"%s\" : does not exist\n", base);
    iput(pip);
    return -5;
  }

  if (DEBUGGING) {
    printf("Parent ino: %d\n, Base ino: %d\n", pip->ino, ino);
  }

  mip = iget(mp->dev, ino); // get the inode to remove

  if (((mip->Inode.i_mode & 0xF000) != 0x8000) && ((mip->Inode.i_mode & 0xF000) != 0xA000))
  {
    printf("\"%s\" : Not a file.\n", base);
    iput(mip);

    if(DEBUGGING)
      printf("\"%s\" : not altered, not a file.\n", base);
  }
  else if (running != &proc[0] && mip->Inode.i_uid != running->uid)
  {
    printf("\"%s\" : Permission denied.\n", base);
    iput(mip);

    if(DEBUGGING)
      printf("\"%s\" : not altered, permission denied.\n", base);

  }
  else if (mip->ref_count > 1) 
  {
    printf("\"%s\" : In use elsewhere.\n", base);
    iput(mip);

    if(DEBUGGING)
      printf("\"%s\" : not altered, in use.\n", pip->name);
  }
  else
  {
    mip->Inode.i_links_count--;
    if (mip->Inode.i_links_count == 0)
    {
      __unlink(mip, pip, base);
    }

    remove_name(pip, mip->ino, base);

    if (DEBUGGING)
      debug_dir(pip);

    pip->Inode.i_atime = time(0L);
    pip->dirty = TRUE;
    mip->dirty = TRUE;
  }

  iput(pip);
  iput(mip);

  return 0;
}

void __unlink (MINODE *toRemove, MINODE *pip, char *name)
{
  int i = 0;
  /*
  while(i < 12 && mip->Inode.i_block[i])  // Deallocate all data blocks
  {
    bdealloc(mip->Inode.i_block[i]);
    mip->Inode.i_block[i] = 0;
    i++;
  }
  */

  _truncate(toRemove);

  mip->Inode.i_size = 0;
  idealloc(mip->ino);                    // Deallocate this inodes inumber 
}
