#include "../include/fs.h"

void map(char buf[], int len)
{
  int  i;

  for (i=0; i < len; i++){
    //(tst_bit(buf, i)) ? putchar('1') : putchar('0');
    if (i && (i % 8)==0)
       printf(" ");
    if (i && (i % 32) == 0)
      putchar('\n');
    (test_bit(buf, i)) ? putchar('1') : putchar('0');
  }
  printf("\n");
}

bool isEmpty(MINODE *at)
{
  if (at->Inode.i_mode != DIRECTORY)
  {
    return FALSE;
  }

  int i;
  char buf[BLKSIZE], *cp, temp[INODE_NAME];
  DIR *dir;

  get_block(mp->dev, at->Inode.i_block[0], buf);
  cp = buf;
  dir = (DIR *)buf;

  while (i < 12 && at->Inode.i_block[i])
  {
    while(cp < buf + BLKSIZE)
    {
      bzero(temp, INODE_NAME);
      strncpy(temp, dir->name, dir->name_len);
      temp[dir->name_len] = 0;
      if (strcmp(temp, ".") != 0 && strcmp(temp, "..") != 0)
      {
        return FALSE;
      }
      if(cp + dp->rec_len  == buf + BLKSIZE)
      {
        break;
      }

      cp += dir->rec_len;
      dir = (DIR *)cp;
    }

    i++;
  }

  return TRUE;
}

void IncFree(bool inode)
{
  char buf[BLKSIZE];

  get_block(mp->dev, SUPERBLK, buf);
  sp = (SUPER *)buf;
  if(inode) 
    sp->s_free_inodes_count++;
  else  
    sp->s_free_blocks_count++;
  put_block(mp->dev, SUPERBLK, buf);

  get_block(mp->dev, SUPERBLK + 1, buf);
  gp = (GD *)buf;
  if(inode)
    gp->bg_free_inodes_count++;
  else  
    gp->bg_free_blocks_count++;
  put_block(mp->dev, SUPERBLK + 1, buf);
}

void decFree(bool inode)
{
  char buf[BLKSIZE];

  /* decrement super block free inode count */
  get_block(mp->dev, SUPERBLK, buf);
  sp = (SUPER *)buf;
  if(inode)
    sp->s_free_inodes_count--;
  else
    sp->s_free_blocks_count--;
  put_block(mp->dev, SUPERBLK, buf);

  /* decrement group desc free inode count */
  get_block(mp->dev, SUPERBLK + 1, buf);
  gp = (GD *)buf;
  if(inode)
    gp->bg_free_inodes_count--;
  else
    gp->bg_free_blocks_count--;
  put_block(mp->dev, SUPERBLK + 1, buf);
}

void _free()
{
  int j = 0;

  if (out) { // free out
    while(j < numTokens) {
      free(out[j]);
      j++;
    }
    free(out);
  }
}