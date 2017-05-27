#include "../include/fs.h"

int enter_name(MINODE *pip, int n_ino, char *name, int type)
{
  char buf[BLKSIZE], *cp;
  int Ideal_Len, n_name_len, remaining, needed;
  int i = 0;

  n_name_len = strlen(name);             // new name's length
  needed = 4 * ((8 + n_name_len + 3) / 4 );  // needed record length

  // Get the last active block
  while(pip->Inode.i_block[i + 1]) 
    i++;

  get_block(pip->dev, pip->Inode.i_block[i], buf);
  cp = buf;
  dp = (DIR *)buf;
  
  // Get the last record item
  while(cp < buf + BLKSIZE) {
    if(cp + dp->rec_len  == buf + BLKSIZE)
      break;

    cp += dp->rec_len;
    dp = (DIR *)cp;
  }

  Ideal_Len = 4 * ((8 + dp->name_len + 3) / 4 );  // last record's ideal length
  remaining = dp->rec_len - Ideal_Len;

  if (DEBUGGING) {
    char temp[256];
    strncpy(temp, dp->name, dp->name_len);
    temp[dp->name_len] = 0;
    printf("Entering new name in parent inode:\n");
    printf("  Name ......................... %s\n", name);
    printf("  Name Length .................. %d\n", n_name_len);
    printf("  Needed Rec Length ............ %d\n", needed);
    printf("  Last Record before new ....... \"%s\"\n", temp);
    printf("  Last Records Ideal Length .... %d\n", Ideal_Len);
    printf("  Remaining Record Length ...... %d\n", remaining);
  }

  if (remaining >= needed) 
  {
    dp->rec_len = Ideal_Len;
    cp += dp->rec_len;
    dp = (DIR *)cp;
  }
  else // MUST allocate a new block first
  {
    int bno = balloc(pip->dev);
    pip->Inode.i_block[i + 1] = bno;

    get_block(pip->dev, bno, buf);
    cp = buf;
    dp = (DIR *)cp;
    i++;                               // Put the new block back instead (one after the current block)
  }

  dp->inode     = n_ino;
  dp->rec_len   = remaining;
  dp->name_len  = n_name_len;
  dp->file_type = type;
  strncpy(dp->name, name, dp->name_len);

  put_block(pip->dev, pip->Inode.i_block[i], buf); // THIS WILL CHANGE WHEN TODO IS ENTERED
}