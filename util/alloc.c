#include "../include/fs.h"

///
/// Used to allocate a free inode from the imap
///
int ialloc(int dev)
{
  int i;
  char buf[BLKSIZE];

  MOUNT *mount_ptr = get_mount(dev);

  get_block(dev, mount_ptr->imap, buf); // read the inode bitmap for this device

  // Test all bits until a free one is found
  for (i = 0; i < mount_ptr->ninodes; i++) { 
    if(test_bit(buf, i) == 0) { // A free inode is found, set it
      set_bit(buf, i);
      decFree(TRUE);
      put_block(dev, mount_ptr->imap, buf);

      if(DEBUGGING) {
        printf("ialloc ->> [%d, %d]\n", dev, i + 1);
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
int balloc(int dev)
{
  int i = 0;
  char buf[BLKSIZE];

  MOUNT *mount_ptr = get_mount(dev);

  get_block(dev, mount_ptr->bmap, buf); // read the block bitmap for this device

  // Test all bits until a free one is found
  while (i < mount_ptr->nblocks) { 
    //printf("i = %d\n", i);
    if(test_bit(buf, i) == 0) { // A free block is found, set it
      set_bit(buf, i);
      decFree(FALSE);
      put_block(dev, mount_ptr->bmap, buf);
      
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