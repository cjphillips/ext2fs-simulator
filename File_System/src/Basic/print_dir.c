#include "../include/fs.h"

static char *t1 = "xwrxwrxwr-------";
static char *t2 = "----------------";

static char *dirColor = "\033[1;34m";
static char *symColor = "\033[1;36m";
static char *exeColor = "\033[1;32m";
static char *broken   = "\033[1;31m";
static char *endColor = "\033[0m";

void print_dir(MINODE *dir)
{
  char *cp, buf[BLKSIZE], temp[256];
  int i, j, at_ino;
  MINODE *at = 0;
  int totalbytes = 0, total = 0;

  for(j = 0; j < 12 && dir->Inode.i_block[j]; j++) {
    if(DEBUGGING) 
      printf("Listing records on block #%d\n", dir->Inode.i_block[j]);
      
    get_block(dir->dev, dir->Inode.i_block[j], buf);
    cp = buf;
    dp = (DIR *)buf;
    
    while (cp < buf + BLKSIZE) {
      strncpy(temp, dp->name, dp->name_len);
      temp[dp->name_len] = 0;
    
      at_ino = dp->inode;
      //if(at_ino == 0) {
      //  if(at)
      //    iput(at);
      //  continue;
      //}
      at = iget(mp->dev, at_ino);
    
      if (DEBUGGING) {
        printf("{DEBUG device: %d: ino = %d, block = %d}\n",
        mp->dev, at_ino, dir->Inode.i_block[j]);
        printf("{DEBUG rec_len = %d\n{DEBUG} name_len = %d}\n", 
        dp->rec_len, dp->name_len);
      }

      if ((at->Inode.i_mode & 0xF000) == 0x8000) // regular file
        putchar('-');
      if ((at->Inode.i_mode & 0xF000) == 0x4000) // directory
        putchar('d');
      if ((at->Inode.i_mode & 0xF000) == 0xA000)
        putchar('l');
    
      for(i = 8; i >= 0; i--) {
        if (at->Inode.i_mode & (1 << i))
          printf("%c", t1[i]);
        else
          printf("%c", t2[i]);
      }

      printf("%4d ", (int)at->Inode.i_links_count);
      printf("%4d ", at->Inode.i_gid);
      printf("%4d ", at->Inode.i_uid);
      printf("%8d ", (int)at->Inode.i_size);

      char *ftime = ctime((time_t *)&at->Inode.i_ctime);
      (ftime) ? printf("%s  ", ftime) : printf("(ctime not found)  ");


      if ((at->Inode.i_mode & 0xF000) == 0xA000) // symbolic link
      {
        char link[BLKSIZE]; bzero(link, BLKSIZE);
        get_block(at->dev, at->Inode.i_block[0], link);

        if (!link)
        {
          printf("%s%s%s -> ", broken, temp, endColor);
        }
        else
        {
          printf("%s%s%s -> ", symColor, temp, endColor);
          printf("%s", basename(link));
        }
      }
      else if ((at->Inode.i_mode & 0xF000) == 0x4000) // directory
        printf("%s%s%s", dirColor, temp, endColor);
      else if ((at->Inode.i_mode & S_IXUSR) == 00100) // executable
        printf("%s%s%s", exeColor, temp, endColor);
      else if ((at->Inode.i_mode & 0xF000) == 0x8000) // normal file
        printf("%s", temp);
    
      putchar('\n');
      iput(at);

      totalbytes += dp->rec_len;
      cp += dp->rec_len;
      dp = (DIR *)cp;
      total++;
    }
  }

  if(DEBUGGING) {
    printf("TOTAL BYTES TRAVERSED  = %d\n", totalbytes);
    printf("TOTAL BLOCKS TRAVERSED = %d\n", totalbytes / BLKSIZE + 1);
  }
  printf("total %d\n", total);
}