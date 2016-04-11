#include "../include/fs.h"

int get_inode(MINODE **ip, char *path)
{
  /* DESTOYS THE CURRENT out VAR */
  int i = 0, j = 0;
  int p_ino, ino;
  char buf[BLKSIZE];

  if(*ip == root)         // Do not iget the root and cwd again
    return 0;
  if(*ip == running->cwd)
    return 0;
  
  if(path[0] == '/') 
    *ip = iget(mp->dev, root->ino);//root;
  else
    *ip = iget(mp->dev, running->cwd->ino);//running->cwd;

  numTokens = tokenize(path, "/");
  if (path[0] == '/')
    numTokens--;

  if(numTokens < 0) // no tokens
    return 0;
 
  while(i < numTokens) {
    if(strcmp(out[i], ".") == 0)
      return 0;
    
    ino = search(*ip, out[i]);
    if(ino < 0) {
      errno = 2; // no such file or directory
      printf("\"%s\" : %s\n", out[i], strerror(errno)); 
      iput(*ip);
      return -1;
    }


    if(DEBUGGING) 
      printf("{DEBUG} parent ino = %d, i_mode = %x\n", ino, (*ip)->Inode.i_mode);
    
    iput(*ip);
    *ip = iget(mp->dev, ino);
    
    if(i < numTokens && !S_ISDIR((*ip)->Inode.i_mode)) {
      errno = 20; // not a directory
      printf("\"%s\" : %s\n", out[i], strerror(errno)); 
      iput(*ip);
      return -2;
    }
    
    i++;
  }
}