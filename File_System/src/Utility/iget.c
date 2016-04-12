#include "../include/fs.h"

static char fname[INODE_NAME];

MINODE *iget(int dev, int ino)
{
  int i;
  
  for(i = 0; i < NMINODES; i++) {
    if (minode[i].ref_count > 0 &&
      minode[i].dev == dev    && 
      minode[i].ino == ino) 
    {
      // An INODE for ino already exists
      minode[i].ref_count++;
      if(DEBUGGING) {
        printf("..........................................\n");
        printf("iget ... found inode[%d, %d] in memory.\n", dev, ino);
        printf(" -> with name \"%s\"\n", minode[i].name);
        printf("..........................................\n");
      }
      return &(minode[i]);
    }
  }

  /* INODE does not exist in memory yet */
  MINODE *mip;
  int blk, offset;
  int p_ino = 0;
  char buf[BLKSIZE];

  i = 0;
  while(minode[i].ref_count > 0)
    i++;
  mip = &minode[i]; // get an inode with no refereces to it

  blk = (ino - 1) / INODES_PER_BLOCK + mp->iblock;
  offset = (ino - 1) % INODES_PER_BLOCK;

  get_block(dev, blk, buf);
  ip = (INODE *)buf + offset;
  
  memcpy(&mip->Inode, ip, sizeof(INODE));
  
  mip->dev = dev;
  mip->ino = ino;
  mip->ref_count = 1;
  mip->dirty = FALSE;

  if(DEBUGGING) {
    printf("..........................................\n");
    printf("iget ... inode found [%d, %d] at block #%d\n", 
     dev, ino, blk);
  }

  
  /* ------ GET THE FILE'S NAME ---------- */
  if(!S_ISDIR(mip->Inode.i_mode)) // dont continue if not a directory
    return mip;

  findino(mip, &ino, &p_ino);    // get the parent's ino

  if (ino == 2 && p_ino == 2) // ignore when initializing root
    return mip;

  blk = (p_ino - 1) / INODES_PER_BLOCK + mp->iblock;
  offset = (p_ino - 1) % INODES_PER_BLOCK;

  get_block(dev, blk, buf);
  ip = (INODE *)buf + offset;    // Contains the parent inode

  findname(ip, ino, fname);
  strcpy(mip->name, fname);      // copy the found name into the mip's name
  if (DEBUGGING) {
    printf("  with name \"%s\", and parent inode %d\n", mip->name, p_ino);
    printf("    *parent ino = 0 means new inode\n");
    printf("..........................................\n");
  }
  /* -------------------------------------*/
  
  return mip;
}