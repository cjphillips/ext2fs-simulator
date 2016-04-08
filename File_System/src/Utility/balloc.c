#include "../include/fs.h"

int _balloc()
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