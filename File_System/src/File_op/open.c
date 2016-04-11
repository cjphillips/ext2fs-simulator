#include "../include/fs.h"

int _open ()
{
  int mode = 0, dev, ino;
  OFT *oftp = 0;
  MINODE *mip;

  if (!out[1])
  {
    printf("No target file specified.\n");
    return -1;
  }
  if (out[2]) // mode has been specified - default is 0 (RD)
  {
    switch(atoi(out[2]))
    {
      case 0: break;
      case 1: mode = 1; break;
      case 2: mode = 2; break;
      case 3: mode = 3; break;
      default:  
        printf ("Invalid mode. (Valid: 0-3)\n");
        return -2;
    }
  }

  if (out[1][0] == '/')  // Get the dev to open from
  {
    dev = root->dev;
  }
  else
  {
    dev = running->cwd->dev;
  }

  ino = get_inode(out[1], &dev);
  mip = iget(dev, ino);

  if (mip->Inode.i_mode != REG_FILE) // Must be a regular file to open
  {
    printf("\"%s\" : not a REG_FILE.\n", mip->name);
    iput(mip);
    return -3;
  }
  // TODO : Check permission of mip to ensure correct access rights

  int i = 0;
  while (i < NFD && oft[i].inode_ptr) // check all file descriptors to ensure this file is not already opened
  {
    if (oft[i].inode_ptr->ino == mip->ino)
    {
      if(oft[i].mode != 0) // A file can be opened many times in read mode (but read mode ONLY).
      {
        printf("\"%s\" : Already opened.\n", mip->name);
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

  if (mode != 0) // Update time fields
  {
    mip->Inode.i_atime = time(0L);
  }
  mip->Inode.i_mtime = time(0L);

  mip->dirty = TRUE;
  // iput(mip)??

  return i;
}