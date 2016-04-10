<<<<<<< Updated upstream
﻿#include "../include/fs.h"

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
=======
﻿
>>>>>>> Stashed changes
