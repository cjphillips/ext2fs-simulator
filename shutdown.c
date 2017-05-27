#include "include/fs.h"

int shutdown ()
{
  int fd = 0, i = 0, j = 0;
  int blk, offset;
  char buf[BLKSIZE];

  printf("Preparing shut down...\n");
  printf("   Writing files back to device ...... ");
  for(i = 0; i < NMINODES; i++) {
    if (minode[i].ref_count > 0) 
    {
      blk = (minode[i].ino - 1) / INODES_PER_BLOCK + mp->iblock;
      offset = (minode[i].ino - 1) % INODES_PER_BLOCK;
      get_block(minode[i].dev, blk, buf);

      ip = (INODE *)buf + offset;
      memcpy(ip, &minode[i].Inode, sizeof(INODE));
      put_block(mp->dev, blk, buf); 
    }
  }
  printf("Done.\n");

  printf("   Closing open file descriptors ..... ");
  while(running->fd[i])
  {
    __close(i);
    i++;
  }
  printf("Done.\n");


  printf("Shut down successful.\n\n");

  get_block(root->dev, SUPERBLK, buf);
  sp = (SUPER *)buf;

  printf("<---------------------------------------------POST CLOSURE INFO---->\n");
  printf("  # of Blocks ......... %d\n", sp->s_blocks_count);
  printf("  # of Free Blocks .... %d\n", sp->s_free_blocks_count);
  printf("  # of Inodes ......... %d\n", sp->s_inodes_count);
  printf("  # of Free Inodes .... %d\n", sp->s_free_inodes_count);
  printf("<------------------------------------------------------------------>\n");

  exit(0);
}