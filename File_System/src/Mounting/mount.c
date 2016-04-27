#include "../include/fs.h"

int mount ()
{
  if (!out[1])
  {
    printf("Usage: mount FILESYSTEM MOUNT_POINT\n");
    return -1;
  }
  if (!out[2])
  {
    printf("Missing mount point.\n");
    return -2;
  }

  char filesys[256], mount_point[1024], buf[BLKSIZE];

  bzero(filesys, 256);
  bzero(mount_point, 1024);

  strcpy(filesys, out[1]);
  strcpy(mount_point, out[2]);

  int i = 0, dev = running->cwd->dev, ino;

  if (out[2][0] == '/')
  {
    dev = root->dev;
  }

  /* Ensure that the passed file system is an EXT2 FS */
  int fd = open(filesys, O_RDWR);
  if (fd < 0)
  {
    printf("\"%s\" : could not open.\n", filesys);
    return -8;
  }
  get_block(fd, SUPERBLK, buf);
  sp = (SUPER *)buf;

  if(sp->s_magic != MAGIC_NUM) { // 0xEF53
    printf("\"%s\" : Not EXT2 FS.\n", basename(filesys));
    close(fd);
    return -4;
  }
  /*-------------------------------------------------- */
  /* Check the validity of the passed mount point      */
  ino = get_inode(mount_point, &dev, FALSE);
  if (ino < 0)
  {
    close(fd);
    return -5;
  }
  MINODE *mip = iget(dev, ino);
  if ((mip->Inode.i_mode & 0xF000) != 0x4000) // Check if the inode is a directory
  {
    printf("\"%s\" : not a directory.\n", basename(mount_point));
    return -6;
  }
  for(i = 0; i < NPROC; i++)
  {

    if (proc[i].status == FREE && (proc[i].cwd && (proc[i].cwd->ino == mip->ino)))
    {
      printf("Mount point in use.\n");
      close(fd);
      return -7;
    }
  }
  /*----------------------------------------------------*/


  i = 0;
  while(mounttab[i].busy == TRUE) // ensure that the passed file system is not already mounted
  {
    if(strcmp(mounttab[i].mount_name, filesys) == 0) 
    {
      printf("\"%s\" : already mounted.\n", basename(filesys));
      return -3;
    }

    i++;
  }

  // i points to an empty mount point
  MOUNT *n_mp = &mounttab[i];
  strncpy(n_mp->mount_name, filesys, 256);
  n_mp->ninodes = sp->s_inodes_count;
  n_mp->nblocks = sp->s_blocks_count;

  n_mp->dev = fd; // will be used to access this device's content

  get_block(fd, sp->s_first_data_block + 1, buf);
  gp = (GD *)buf;

  n_mp->imap = gp->bg_inode_bitmap;
  n_mp->bmap = gp->bg_block_bitmap;
  n_mp->iblock = gp->bg_inode_table;

  n_mp->mounted_inode = mip;
  n_mp->busy = TRUE;
  mip->mount_ptr = n_mp;
  mip->mounted = TRUE;

  return 0;
}