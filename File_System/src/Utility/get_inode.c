#include "../include/fs.h"

int get_inode(char *path, int *device)
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

    iput(ip);
    ip = iget(*device, ino);
    
    ino = search(ip, out[i]);
    if(ino < 0) {
      errno = 2; // no such file or directory
      printf("\"%s\" : %s\n", out[i], strerror(errno)); 
      iput(ip);
      return -1;
    }

    if(DEBUGGING) 
      printf("{DEBUG} parent ino = %d, i_mode = %x\n", ino, ip->Inode.i_mode);
    
    if(i < numTokens && ip->Inode.i_mode != DIRECTORY) {
      errno = 20; // not a directory
      printf("\"%s\" : %s\n", out[i], strerror(errno)); 
      iput(ip);
      return -2;
    }
    
    i++;
  }

  iput(ip);
  return ino;
}