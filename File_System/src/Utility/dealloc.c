#include "../include/fs.h"

///
/// Used to deallocate an in-use inode from the imap
///
void idealloc(int ino, int dev)
{
  int i;
  char buf[BLKSIZE];

  MOUNT *mount_ptr = get_mount(dev);

  get_block(dev, mount_ptr->imap, buf);

  clear_bit(buf, ino - 1);
  IncFree(TRUE);

  put_block(dev, mount_ptr->imap, buf);
}

void bdealloc(int bno, int dev)
{
  char buf[BLKSIZE];

  MOUNT *mount_ptr = get_mount(dev);

  get_block(dev, mount_ptr->bmap, buf);

  clear_bit(buf, bno - 1);
  IncFree(FALSE);

  put_block(dev, mount_ptr->bmap, buf);
}