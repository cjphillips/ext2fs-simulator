#include "../include/fs.h"

void __mkdir  (MINODE *pip, char *name);

int _mkdir()
{
  if(numTokens < 2) { // no name provided
    printf("No target provided.\n");
    return -1;
  }
  if(strcmp(out[1], "/") == 0) { // cannot make root
    printf("\"/\" :  already exists.\n");
    return -4;
  }

  int dev;
  int p_ino;
  MINODE *pip;
  char buf[BLKSIZE];
  
  char base[INODE_NAME], dirs[INODE_NAME*6];

  if(out[1][0] == '/')
  {
    dev = root->dev;
  }
  else
  {
    dev = running->cwd->dev;
  }

  strcpy(base, basename(out[1])); // the target name
  strcpy(dirs, dirname (out[1])); // the parent path
  dirs[strlen(dirs)] = 0;

  if (DEBUGGING)
    printf("Path : %s\nBase : %s\n", dirs, base);

  // get the parent's inode, starting from the root OR the cwd
  if((p_ino = get_inode(dirs, &dev)) < 0) 
  {
    return p_ino;
  }

  pip = iget(dev, p_ino);

  // Make sure that the new dir does not already exist
  if((search(pip, base) > 0)) { 
    printf("\"%s\" : Already exists.\n", base);
      return -5;
  }

  __mkdir(pip, base);

  if (DEBUGGING)
    debug_dir(pip);

  pip->Inode.i_links_count++;    
  pip->Inode.i_atime = time(0L);
  pip->dirty = TRUE;

  iput(pip);
      
  return 0;
}

void __mkdir(MINODE *pip, char *name)
{
  char buf[BLKSIZE];

  int n_bno = balloc();
  int n_ino = ialloc();
  int i;

  mip = iget(pip->dev, n_ino);
  ip = &mip->Inode;

  if (DEBUGGING) {
    printf("Creating new inode under [%d, %d]\n", mp->dev, pip->ino);
  }

  /* Set the new inode's contents */
  ip->i_mode = 0x41ED;                        // DIR type
  ip->i_uid = running->uid;                   // User ID
  ip->i_gid = running->gid;                   // Group ID
  ip->i_size = BLKSIZE;                         // constant block size
  ip->i_links_count = 2;                      // itself AND its parent
  ip->i_atime = ip->i_ctime = time(0L);
  ip->i_mtime = ip->i_ctime;                  // time fields to current time
  ip->i_blocks = 2;                           // 2 * 512MB = 1024 = BLKSIZE
  ip->i_block[0] = n_bno;                     // data blocks start at n_bno
  for(i = 1; i < 15; i++)
    ip->i_block[i] = 0;                       // All other data blocks are empty

  strcpy(mip->name, name);                    // copy the new directory's name
  mip->dirty = TRUE;                          // marked to write back to disk
  
  iput(mip);                                  // Write new inode back to disk
  
  /* Set block contents (initial data block '.' and '..' */
  char *cp;

  get_block(mp->dev, n_bno, buf);      // Read block
  cp = buf;
  dp = (DIR *)buf;

  // ->> current inode
  dp->inode     = n_ino;               // Set the new inode number
  dp->rec_len   = 12;                  // Set this record's len
  dp->name_len  = 1;                   // '.' is only one char long
  dp->file_type = DIRECTORY;           // DIR type file
  strncpy(dp->name, ".", 1);           // write the name

  cp += dp->rec_len;
  dp = (DIR *)cp;
  
  // ->> parent inode
  dp->inode     = pip->ino;            // set parent's inode
  dp->rec_len   = 1012;                // the rest of the block
  dp->name_len  = 2;                   // ".." is only two char long
  dp->file_type = DIRECTORY;           // parent is a directory
  strncpy(dp->name, "..", 2);          // write the name
  
  put_block(mp->dev, n_bno, buf);      // Write block back

  enter_name(pip, n_ino, name, 0x41ED);
}
