#include "../include/fs.h"

int mv ()
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
    printf("Cannot move a file into itself.\n");
    return -3;
  }

  int ino, p_ino, sp_ino, src_dev = running->cwd->dev, dest_dev = running->cwd->dev, src_parent_dev = src_dev;
  char src[INODE_NAME*2], dest[INODE_NAME*2], dest_parent_path[INODE_NAME*2], src_parent_path[INODE_NAME*2];

  strcpy(src, out[1]);
  strcpy(dest, out[2]);
  strcpy(src_parent_path, dirname(out[1]));
  strcpy(dest_parent_path, dirname(out[2]));

  if (out[2][0] == '/')
  {
    src_parent_dev = src_dev = root->dev;
    dest_dev = root->dev;
  }

  ino = get_inode(dest, &dest_dev, TRUE);       // Make sure that the destination file does not already exist
  if (ino > 0)                                    // Destination file already exists
  {
    printf("\"%s\" : file already exists with this name.\n", dest);
    return -4;
  }
  p_ino = get_inode(dest_parent_path, &dest_dev, TRUE); // Make sure that the destination's parent exists
  if (p_ino < 0)
  {
    printf("\"%s\" : parent directory does not exist.\n", basename(dest_parent_path));
    return -4;
  }
  ino = get_inode(src, &src_dev, FALSE);       // Make sure that the source file does exist
  if (ino < 0)
  {
    return ino;
  }

  sp_ino = get_inode(src_parent_path, &src_parent_dev, TRUE);

  MINODE *mip   = iget(src_dev, ino);
  MINODE *d_pip = iget(dest_dev, p_ino);
  MINODE *s_pip = iget(src_dev, sp_ino);

  if (src_dev == dest_dev) // the source and destination are on the same device
  {
    enter_name(d_pip, ino, basename(dest), mip->Inode.i_mode);

    d_pip->dirty = TRUE;
    mip->dirty = TRUE;

    remove_name(s_pip, mip->ino, basename(src));
  }
  // TODO: moving files across devices
  iput(mip);
  iput(d_pip);
  iput(s_pip);

  return 0;
}