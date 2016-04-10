#include "../include/fs.h"

///
/// Used to allocate a free inode from the imap
///
int ialloc()
{
  int i;
  char buf[BLKSIZE];

  get_block(mp->dev, mp->imap, buf); // read the inode bitmap for this device

  // Test all bits until a free one is found
  for (i = 0; i < mp->ninodes; i++) { 
    if(test_bit(buf, i) == 0) { // A free inode is found, set it
      set_bit(buf, i);
      decFree(TRUE);
      put_block(mp->dev, mp->imap, buf);

      if(DEBUGGING) {
        printf("ialloc ->> [%d, %d]\n", mp->dev, i + 1);
      }

      return i + 1; // return ino of the allocated block
    }
  }

  if(DEBUGGING) {
    printf("ialloc ->> No free inodes.\n");
  }

  return 0; // no free inodes left
}

///
/// Used to allocate a free block from the bmap
///
int balloc()
{
  int i = 0;
  char buf[BLKSIZE];

  get_block(mp->dev, mp->bmap, buf); // read the block bitmap for this device

  // Test all bits until a free one is found
  while (i < mp->nblocks) { 
    //printf("i = %d\n", i);
    if(test_bit(buf, i) == 0) { // A free block is found, set it
      set_bit(buf, i);
      decFree(FALSE);
      put_block(mp->dev, mp->bmap, buf);
      
      if(DEBUGGING) {
  printf("balloc ->> new data block at #%d\n", i + 1);
      }

      return i + 1; // return bno of the allocated block
    }

    i++;
  }

  if(DEBUGGING) {
    printf("balloc ->> No free blocks.\n");
  }

  return 0; // no free blocks left
}