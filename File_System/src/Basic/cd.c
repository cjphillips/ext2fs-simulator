#include "../include/fs.h"

int cd()
{
  bool from_root;

  if (!out[1]) { // Changing to home directory
    running->cwd = root;
    return 0;
  }
  if (strcmp(out[1], "/") == 0) {    // Return to home
    running->cwd = root;
    return 0;
  }
  if (strcmp(out[1], ".") == 0) {    // No need to change directory
    return 0;
  }
   
  char *base = basename(out[1]);  // target directory
  char buf[BLKSIZE];
  int i = 0, ino;

  if(out[1][0] == '/')
    mip = root;
  else
    mip = running->cwd;

  if (out[1][0] == '/')
    from_root = TRUE;

  numTokens = tokenize(out[1], "/");
  if(from_root)
    numTokens--;
  
  while(i < numTokens) {
    if (i < (numTokens - 1) && !S_ISDIR(mip->Inode.i_mode)) {
      printf("\"%s\" : Not a directory\n", out[i]);
      if(mip != root && mip != running->cwd)
  iput(mip);
      return -2;
    }

    ino = search(mip, out[i]);

    if(ino < 0) { // Inode not found
      printf("\"%s\" : Does not exist.\n", out[i]);
      if(mip != root && mip != running->cwd)
  iput(mip);
      return -1;
    }
    
    if(mip != root && mip != running->cwd)
      iput(mip);
    
    mip = iget(mp->dev, ino);
    i++;
  }

  if (!S_ISDIR(mip->Inode.i_mode)) { // not a directory
    printf("\"%s\" : Not a directory.\n", base);
    return -3;
  }

  running->cwd = mip;
  iput(mip);
    
  return 0;
}