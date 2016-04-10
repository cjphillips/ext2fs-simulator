#include "../include/fs.h"

///
/// Used to deallocate an in-use inode from the imap
///
void idealloc(int ino)
{
  int i;
  char buf[BLKSIZE];

  get_block(mp->dev, mp->imap, buf);

  clear_bit(buf, ino - 1);
  IncFree(TRUE);

  put_block(mp->dev, mp->imap, buf);
}

void bdealloc(int bno)
{
  char buf[BLKSIZE];

  get_block(mp->dev, mp->bmap, buf);

  clear_bit(buf, bno - 1);
  IncFree(FALSE);

  put_block(mp->dev, mp->bmap, buf);
}