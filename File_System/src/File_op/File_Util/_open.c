#include "../../include/fs.h"

int __open (char *file, int mode)
{
  int dev, ino;
  OFT *oftp = 0;
  MINODE *mip;

  if (file[0] == '/')  // Get the dev to open from
  {
    dev = root->dev;
  }
  else
  {
    dev = running->cwd->dev;
  }

  ino = get_inode(file, &dev);

  if (ino < 0)
  {
    return ino;
  }

  mip = iget(dev, ino);

  if (mip->Inode.i_mode != REG_FILE) // Must be a regular file to open
  {
    if (mip->Inode.i_mode == LNK)    // Get the name the link points to
    {
      printf("Not yet implemented for symbolic links.\n");
      iput(mip);
      return -100;
      /*
      char link[60];
      get_block(dev, mip->Inode.i_block[0], link);
      link[60] = 0;
      file = link;
      iput(mip);
      ino = get_inode(link, &dev);
      if (ino < 0)
      {
        return ino;
      }
      mip = iget(dev, ino);
      */
    }

    if (mip->Inode.i_mode != REG_FILE)
    {
      printf("\"%s\" : not a file.\n", file);
      iput(mip);
      return -3;
    }
  }
  // TODO : Check permission of mip to ensure correct access rights

  int i = 0;
  while (i < NFD && oft[i].inode_ptr) // check all file descriptors to ensure this file is not already opened
  {
    if (oft[i].inode_ptr->ino == mip->ino)
    {
      if(mode != 0) // A file can be opened many times in read mode (but read mode ONLY).
      {
        printf("\"%s\" : Already opened.\n", file);
        iput(mip);
        return -4;
      }
    }

    i++;
  }


  i = 0;
  oftp = &oft[0];
  while(i < NFD) // Find a free file descriptor
  {
    if(oft[i].ref_count == 0)
      break;
    i++;
    oftp = &oft[i];
  }
  if (i == NFD) // No file descriptors are available
  {
    printf("Unable to open file: too many files are already opened.\n");
    iput(mip);
    return -5;
  }


  oftp->mode = mode;     // The desired mode
  oftp->ref_count = 1;   // This file is being referenced once
  strcpy(mip->name, file);
  oftp->inode_ptr = mip; // Pointer to the files inode

  switch(mode) // Set the offset for the file accordingly
  {
    case 0: oftp->offset = 0; break;                 // READ: begin reading at the beginning of the file
    case 1: oftp->offset = 0; truncate(mip); break;  // WRITE: Remove the file's contents and begin writing at beginning.
    case 2: oftp->offset = 0; break;                 // READ-WRITE: Read from the beginning of the file, but do not remove contents.
    case 3: oftp->offset = mip->Inode.i_size; break; // APPEND: Begin writing at the end of the file.
    default: iput(mip); return -2;                   // The default case should never be hit; already handled above.
  }

  i = 0;
  while(i < NFD && running->fd[i]) // Get the first free file descriptor from the running process
  {
    i++;
  }
  if (i == NFD) // No free descriptors left
  {
    // TODO: close the file
  }

  running->fd[i] = oftp;

  if (mode != 0) // Update time fields
  {
    mip->Inode.i_atime = time(0L);
  }
  mip->Inode.i_mtime = time(0L);

  mip->dirty = TRUE;
  // iput(mip)??

  return i;
}