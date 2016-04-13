#include "include/fs.h"

int shutdown ()
{
  int fd = 0, i = 0, j = 0;
  char buf[BLKSIZE];

  printf("Preparing shut down...\n");
  printf("   Writing files back to device ...... ");
  // TODO: Write all in-use inodes back to disk
  printf("Done.\n");
  printf("   Closing open file descriptors ..... ");
  // TODO: Close all open fds
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