#include "../include/fs.h"

int get_inode(char *path, int *device, bool quiet)
{
  /* DESTOYS THE CURRENT out VAR */
  int i = 0, j = 0;
  int p_ino, ino;
  char buf[BLKSIZE];
  bool from_root = FALSE;
  MINODE *ip = 0;

  /*
  if(*ip == root)         // Do not iget the root and cwd again
    return 0;
  if(*ip == running->cwd)
    return 0;
  */

  if(path[0] == '/')
    from_root = TRUE;
  
  if(path[0] == '/') 
  {
    ino = root->ino;
    ip = iget(*device, ino);
  }
  else
  {
    ino = running->cwd->ino;
    ip = iget(*device, ino);
  }

  numTokens = tokenize(path, "/");
  if (from_root)
    numTokens--;

  if(numTokens < 0) // no tokens
  {
    iput(ip);
    return ino;
  }
 
  while(i < numTokens) {
    if(strcmp(out[i], ".") == 0)
    {
      iput(ip);
      return ino;
    }
    
    ino = search(ip, out[i]);
    iput(ip);

    if(ino < 0) {
      errno = 2; // no such file or directory
      if (!quiet)
        printf("\"%s\" : %s.\n", out[i], strerror(errno)); 
      //iput(ip);
      return -1;
    }
   
    ip = iget(*device, ino);

    if (ip->mounted) // this *directory* is mounted by a device (DOWNWARD TRAVERSAL)
    {
      *device = ip->mount_ptr->dev; // The device has changed - this number will be needed outside of this function as well

      iput(ip);
      ip = iget(*device, 2);        // Get the root inode from the mounted device

      ino = ip->ino;
    }
    else if (ip->ino == 2 && ip->dev != root->dev) // This is the root of the current mounted device (UPWARD TRAVERSAL)
    {
      MINODE *mount_mip = 0;
      while(j < NMOUNT)                            // Search for this device in the mount tab
      {
        if (mounttab[j].dev == ip->dev)
        {
          mount_mip = mounttab[j].mounted_inode;
          break;
        }

        j++;
      }

      *device = mount_mip->dev;                    // Change to this inodes device

      iput(ip);
      ip = iget(*device, mount_mip->ino);
      ino = ip->ino;
      if (strcmp(out[i], "..") == 0)               // Have crossed into the parent device.. must now traverse one more time
      {
        ino  = search(ip, "..");
      }
    }

    if(DEBUGGING) 
      printf("{DEBUG} parent ino = %d, i_mode = %x\n", ino, ip->Inode.i_mode);
    
    if(i + 1 < numTokens && ip->Inode.i_mode != DIRECTORY) {
      errno = 20; // not a directory
      if (!quiet)
        printf("\"%s\" : %s\n", out[i], strerror(errno)); 
      iput(ip);
      return -2;
    }
    
    i++;
  }

  iput(ip);
  return ino;
}