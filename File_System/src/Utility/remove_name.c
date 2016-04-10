#include "../include/fs.h"

void remove_name(MINODE *pip, int d_ino, char *name)
{
  bool found = FALSE;
  int i = 0;
  char buf[BLKSIZE], *cp, temp[INODE_NAME];
  DIR *prev_dp = 0;

  cp = buf;
  dp = (DIR *)buf;


  // Find record
  while (i < 12 && pip->Inode.i_block[i])
  {
    get_block(mp->dev, pip->Inode.i_block[i], buf);
    while(cp < buf + BLKSIZE) 
    {
      strncpy(temp, dp->name, dp->name_len);
      temp[dp->name_len] = 0;
      if (strcmp(temp, name) == 0)
      {
        found = TRUE;
        break;
      }
      if (cp + dp->rec_len == buf + BLKSIZE)
        break;

      cp += dp->rec_len;
      prev_dp = dp;
      dp = (DIR *)cp;
    }

    if(found)
      break;

    i++;
    prev_dp = 0;
  }

  if(DEBUGGING) 
    printf("Record Found: %s, Blk #%d\n", temp, pip->Inode.i_block[i]);

  /* Find position of record in block */
  if (cp + dp->rec_len >= BLKSIZE) { // LAST RECORD
    dp->inode = 0;                     // zero out ino
    prev_dp->rec_len += dp->rec_len; // have prev record's length extend over the rest of the record
    put_block(mp->dev, pip->Inode.i_block[i], buf);
  }
  else                               // Could be first, or in-between records
  {
    dp->inode = 0;
    if (prev_dp = 0)                 // first record
    {
      bdealloc(pip->Inode.i_block[i]); // Remove this block
      put_block(mp->dev, pip->Inode.i_block[i], buf);

      while(pip->Inode.i_block[i + 1]) // Move all blocks down
      {
        pip->Inode.i_block[i] = pip->Inode.i_block[i + 1];
      }
      pip->Inode.i_block[i] = 0;
      pip->Inode.i_size -= BLKSIZE;
    }
    else                             // In-between record
    {
      int ideal = dp->rec_len;

      prev_dp = dp;
      cp += dp->rec_len;
      dp = (DIR *)cp;

      while(cp < buf + BLKSIZE)
      {
        if(cp + dp->rec_len != buf + BLKSIZE)
          break;

        prev_dp->inode     = dp->inode;
        prev_dp->rec_len   = dp->rec_len;
        prev_dp->name_len  = dp->name_len;
        prev_dp->file_type = dp->file_type;

        strncpy(prev_dp->name, dp->name, dp->name_len);

        prev_dp = dp;
        cp += dp->rec_len;
        dp = (DIR *)cp;
      }
      put_block(mp->dev, pip->Inode.i_block[i], buf);
    }
  }
}