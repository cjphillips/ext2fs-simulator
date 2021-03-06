﻿#include "../include/fs.h"

int _creat()
{
  if(numTokens < 2) { // no name provided
    printf("\"creat\" : No target specified.\n");
    return -1;
  }
  if(strcmp(out[1], "/") == 0) { // cannot make root
    printf("\"/\" : cannot create file with this name.\n");
    return -4;
  }
 
  int dev = running->cwd->dev;
  int p_ino;
  MINODE *pip;
  char buf[BLKSIZE];

  char base[INODE_NAME], dirs[INODE_NAME*6];

  if (out[1][0] == '/')
  {
    dev = root->dev;
  }

  strcpy(base, basename(out[1])); // the target name
  strcpy(dirs, dirname (out[1])); // the parent path
  dirs[strlen(dirs)] = 0;

  if (DEBUGGING)
    printf("Path : %s\nBase : %s\n", dirs, base);

  // get the parent's inode, starting from the root OR the cwd
  if((p_ino = get_inode(dirs, &dev, FALSE)) < 0) 
  {
    return p_ino;
  }

  pip = iget(dev, p_ino);

  // Make sure that the new dir does not already exist
  if((search(pip, base) > 0)) { 
    printf("\"%s\" : Already exists.\n", base);
    return -5;
  }

  __creat(pip, base, REG_FILE);

  if (DEBUGGING)
    debug_dir(pip);
  
  pip->Inode.i_mtime = time(0L);
  pip->dirty = TRUE;

  iput(pip);
      
  return 0;
}

void __creat(MINODE *pip, char *name, int type)
{
  char buf[BLKSIZE];

  int n_ino = ialloc(pip->dev);  // Allocate a new inode
  int i;

  mip = iget(pip->dev, n_ino);
  ip = &mip->Inode;

  /* Set the new inode's contents */
  ip->i_mode = type;                          // REG_FILE type
  ip->i_uid = running->uid;                   // User ID
  ip->i_gid = running->gid;                   // Group ID
  ip->i_size = 0;                             // no blocks, no size
  ip->i_links_count = 1;                      // itself AND its parent
  ip->i_atime = ip->i_mtime = time(0L);
  ip->i_ctime = ip->i_mtime;                  // time fields to current time
  ip->i_blocks = 0;                           // initially no data blocks
  ip->i_block[0] = 0;                         // "         ...          "
  for(i = 1; i < 15; i++)
    ip->i_block[i] = 0;                       // All other data blocks are empty

  strcpy(mip->name, name);                    // copy the new directory's name
  mip->dirty = TRUE;                          // marked to write back to disk
  
  iput(mip);                                  // Write new inode back to disk
  
  enter_name(pip, n_ino, name, REG_FILE);
}