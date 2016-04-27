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

  return _cp(out[1], out[2]);
}


int _cp(char *src, char *dest)
{
  int fd, gd, ino, p_ino, dev = running->cwd->dev;
  char src_path[INODE_NAME], dest_path[INODE_NAME], parent_path[INODE_NAME*2];
  MINODE *pip;

  strcpy(src_path, src);
  strcpy(dest_path, dest);
  strcpy(parent_path, dirname(dest));

  if (dest[0] == '/')
  {
    dev = root->dev;
  }

  ino = get_inode(dest, &dev, TRUE);
  if (ino < 0)                                    // Must first create the destination file
  {
    p_ino = get_inode(parent_path, &dev, FALSE);
    if (p_ino < 0)
    {
      return p_ino;
    }
    pip = iget(dev, p_ino);
    __creat(pip, dest_path, REG_FILE);
    iput(pip);
  }

  fd = __open(src_path, 0); // open the source file for read
  if (fd < 0)
  {
    return fd;
  }
  gd = __open(dest_path, 1); // open the dest file for write
  if (gd < 0)
  {
    return gd;
  }

  int n = 0, counter = 0;
  char cp_buf[BLKSIZE];
  bzero(cp_buf, BLKSIZE);

  while ( (n = __read(fd, cp_buf, BLKSIZE)) > 0 ) // Read from the source file
  {
    if (n < 0)
      break;

    __write(gd, cp_buf, n);                       // Write to the destination file

    bzero(cp_buf, BLKSIZE);
    counter++;
  }

  __close(fd);
  __close(gd);

  return 0;
}