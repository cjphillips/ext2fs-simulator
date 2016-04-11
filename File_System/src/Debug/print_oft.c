#include "../include/fs.h"

int print_oft()
{
  int i = 0;
  OFT *oftp = 0;

  printf("File Desc. for Processs #%d\n", running->uid);
  printf("fd  mode  count  offset  [dev, ino]  filename\n");
  printf("--  ----  -----  ------  ----------  --------\n");

  while(i < NFD)
  {
    if(running->fd[i])
    {
      oftp = running->fd[i];
      printf("%2d    %2d      %d         %4d     [%d, %d]      %s\n", i, oftp->mode, oftp->ref_count, oftp->offset, 
                                                                      oftp->inode_ptr->dev, oftp->inode_ptr->ino, oftp->inode_ptr->name);
    }
    i++;
  } 
}