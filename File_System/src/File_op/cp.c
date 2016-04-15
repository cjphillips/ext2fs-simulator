#include "../include/fs.h"

int cp ()
{
  if (!out[1])
  {
    printf("Usage: cp SRC DEST\n");
    return -1;
  }
  if (!out[2])
  {
    printf("Missing target destination.\n");
    return -2;
  }
  if (strcmp(out[1], out[2]) == 0)
  {
    printf("Cannot copy a file to itself.\n");
    return -3;
  }

  int fd, gd, ino, p_ino, dev = running->cwd->dev;
  char src[INODE_NAME*2], dest[INODE_NAME*2], parent_path[INODE_NAME*2];
  MINODE *pip;

  strcpy(src, out[1]);
  strcpy(dest, out[2]);
  strcpy(parent_path, dirname(out[2]));

  if (out[2][0] == '/')
  {
    dev = root->dev;
  }

  ino = get_inode(out[2], &dev, TRUE);
  if (ino < 0)                                    // Must first create the destination file
  {
    p_ino = get_inode(parent_path, &dev, FALSE);
    if (p_ino < 0)
    {
      return p_ino;
    }
    pip = iget(dev, p_ino);
    __creat(pip, dest, REG_FILE);
    iput(pip);
  }

  fd = __open(src, 0); // open the source file for read
  if (fd < 0)
  {
    return fd;
  }
  gd = __open(dest, 1); // open the dest file for write
  if (gd < 0)
  {
    return gd;
  }

  int n = 0;
  char cp_buf[BLKSIZE];
  bzero(cp_buf, BLKSIZE);

  while ( (n = __read(fd, cp_buf, BLKSIZE)) > 0 ) // Read from the source file
  {
    if (n < 0)
      break;
    __write(gd, cp_buf, n);                       // Write to the destination file
    bzero(cp_buf, BLKSIZE);
  }

  __close(fd);
  __close(gd);

  return 0;
}