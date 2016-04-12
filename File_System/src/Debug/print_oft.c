#include "../include/fs.h"

static char *Color_Begin_Proc = "\033[1;33m";
static char *Color_End = "\033[0m";

int print_oft ()
{
  int i = 0;
  OFT *oftp = 0;

  printf("File Desc. for [%sProcesss %d%s]:\n", Color_Begin_Proc, running->uid, Color_End);
  printf("FD  Mode  Count  Offset  [dev, ino]  Filename\n");
  printf("--  ----  -----  ------  ----------  --------\n");

  while(i < NFD)
  {
    if(running->fd[i])
    {
      oftp = running->fd[i];
      printf("%2d  ", i);
      switch(oftp->mode)
      {
        case 0:  printf("READ    "); break;
        case 1:  printf("WRIT    "); break;
        case 2:  printf("RDWR    "); break;
        case 3:  printf("APPD    "); break;
        default: printf("DFLT    "); break;
      }

      printf("%d    ", oftp->ref_count);
      printf("%6ld   ", oftp->offset);
      printf("[%2d, %3d]   ", oftp->inode_ptr->dev, oftp->inode_ptr->ino);
      printf("%s\n", oftp->inode_ptr->name);
    }
    i++;
  } 
}