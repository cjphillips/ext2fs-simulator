#include "include/fs.h"

static char *t1 = "xwrxwrxwr-------";
static char *t2 = "----------------";

static char *dirColor = "\033[1;34m";
static char *symColor = "\033[1;36m";
static char *exeColor = "\033[1;32m";
static char *endColor = "\033[0m";

void print_dir(MINODE *dir)
{
  char *cp, buf[BLKSIZE], temp[256];
  int i, j, at_ino;
  MINODE *at;
  int totalbytes = 0, total = 0;

  for(j = 0; j < 12 && dir->Inode.i_block[j]; j++) {
    if(DEBUGGING) 
      printf("Listing directories on block #%d\n", dir->Inode.i_block[j]);
      
    get_block(dir->dev, dir->Inode.i_block[j], buf);
    cp = buf;
    dp = (DIR *)buf;
    
    while (cp < buf + BLKSIZE) {
      strncpy(temp, dp->name, dp->name_len);
      temp[dp->name_len] = 0;
    
      at_ino = dp->inode;
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

      if ((at->Inode.i_mode & 0xF000) == 0x4000) // directory
	printf("%s%s%s", dirColor, temp, endColor);
      else if ((at->Inode.i_mode & 0xF000) == 0xA000) // symbolic link
	printf("%s%s%s", symColor, temp, endColor);
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

int ls()
{
  char buf[BLKSIZE];
  int i;

  if(!out[1]) // printing the current working directory
    mip = running->cwd;
  // TODO: get file path

  if (S_ISDIR(mip->Inode.i_mode)) { // printing directory
    print_dir(mip);
    return 0;
  }
  else {
    printf("\"%s\" : Not a directory.\n", out[1]);
  }
  
  return -1;
}

void rpwd(MINODE *at)
{
  char buf[BLKSIZE];
  int p_ino, dump;

  if (at == root) {
    return;
  }

  get_block(mp->dev, at->Inode.i_block[0], buf);
  findino(at, &dump, &p_ino);

  rpwd(iget(mp->dev, p_ino));
  printf("/%s", at->name);
}

int pwd()
{
  char buf[BLKSIZE];
  int p_ino;

  mip = running->cwd;

  if(mip == root) {
    printf("/\n");
    return 0;
  }

  rpwd(mip);
  printf("\n");

  //iput(mp->dev, p_ino);

  return 0;
}

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
