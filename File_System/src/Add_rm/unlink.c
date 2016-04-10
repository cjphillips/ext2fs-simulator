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

  int dev;
  int ino;
  MINODE *pip;
  char buf[BLKSIZE];
  
  char base[INODE_NAME], dirs[INODE_NAME*6];

  strcpy(base, basename(out[1])); // the target name
  strcpy(dirs, dirname (out[1])); // the parent path
  dirs[strlen(dirs)] = 0;

  if (DEBUGGING)
    printf("Path : %s\nBase : %s\n", dirs, base);

  // get the parent's inode, starting from the root OR the cwd
  int r;
  if ((r = get_inode(&pip, dirs)) < 0) {
    return r;
  }

  ino = search(pip, base);

  if (DEBUGGING) {
    printf("Parent ino: %d\n, Base ino: %d\n", pip->ino, ino);
  }

  mip = iget(mp->dev, ino); // get the inode to remove

  if (mip->Inode.i_mode != REG_FILE)
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
  else if (mip->Inode.i_links_count > 1) 
  {
    printf("\"%s\" : In use elsewhere.\n", base);
    iput(mip);

    if(DEBUGGING)
      printf("\"%s\" : not altered, in use.\n", pip->name);
  }
  else
  {
    __unlink(mip, pip, base);

    if (DEBUGGING)
      debug_dir(pip);

    pip->Inode.i_links_count--;
    pip->Inode.i_atime = time(0L);
    pip->dirty = TRUE;
  }

  iput(pip);
  iput(mip);

  return 0;
}

void __unlink (MINODE *toRemove, MINODE *pip, char *name)
{
  int i = 0;

  while(i < 12 && mip->Inode.i_block[i])  // Deallocate all data blocks
  {
    bdealloc(mip->Inode.i_block[i]);
    i++;
  }
  idealloc(mip->ino);                    // Deallocate this inodes inumber 

  remove_name(pip, toRemove->ino, name);
}
