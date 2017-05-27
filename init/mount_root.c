#include "../include/fs.h"

int mount_root()
{
  int i, ino, fd, dev;
  MINODE *ip;
  bool success = FALSE;
  
  char line[64], buf[BLKSIZE];
  int ninodes, nblocks, ifree, bfree;

  do {
    do {
      printf("Enter root device: ");
      fgets(line, 64, stdin);
    } while (line[0] == '\n');
    line[strlen(line) - 1] = 0;

    printf("Mounting Root ... ");

    root_dev = line; // the device to be operated on

    dev = open(root_dev, O_RDWR);  // system's open! not to be confused with _open
    if(dev < 0) {
      printf("Failed.\n");
      printf("Could not open \"%s\".\n", root_dev);
      printf("Try Again.\n\n");
    }
    else 
      success = TRUE;
  } while(!success);

  /* Get Super Block */
  get_block(dev, SUPERBLK, buf);
  sp = (SUPER *)buf;

  /* Verify Ext2 FS */
  if(sp->s_magic != MAGIC_NUM) { // 0xEF53
    printf("\"%s\" : Not EXT2 FS.\n", root_dev);
    exit(2);
  }

  mp = &mounttab[0];
  
  /* copy super blocks info into mounttab[0] */
  ninodes = mp->ninodes = sp->s_inodes_count;
  nblocks = mp->nblocks = sp->s_blocks_count;
  bfree = sp->s_free_blocks_count;
  ifree = sp->s_free_inodes_count;

  get_block(dev, 1 + sp->s_first_data_block, buf);
  gp = (GD *)buf;
  
  mp->dev = dev;
  mp->busy = TRUE;

  mp->bmap   = gp->bg_block_bitmap;
  mp->imap   = gp->bg_inode_bitmap;
  mp->iblock = gp->bg_inode_table;
  
  strcpy(mp->name, root_dev);
  strcpy(mp->mount_name, "/");

  /* Call iget() */
  root = iget(dev, 2);       // Get Root's Inode
  strcpy(root->name, "/");

  mp->mounted_inode = root;  
  root->mount_ptr = mp;

  printf("Successful.\n\n");
  printf("<---------------------------------------------------DEVICE INFO---->\n");
  printf(" Device \"%s\" : Mounted on /\n", root_dev);
  printf("  # of Blocks ......... %d\n", nblocks);
  printf("  # of Free Blocks .... %d\n", bfree);
  printf("  # of Inodes ......... %d\n", ninodes);
  printf("  # of Free Inodes .... %d\n", ifree);
  printf("<------------------------------------------------------------------>\n\n");

  return 0;
}