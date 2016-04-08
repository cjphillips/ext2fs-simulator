#include "include/fs.h"

extern char *cmds[];
static char fname[256];


void get_block(int fd, int blk, char buf[])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  read(fd, buf, BLKSIZE);
}

void put_block(int fd, int blk, char buf[])
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  write(fd, buf, BLKSIZE);
}

int findino(MINODE *mip, int *ino, int *p_ino)
{
  char buf[BLKSIZE], *cp;
  DIR *ddp;
  int i;

  get_block(mp->dev, mip->Inode.i_block[0], buf);
  cp = buf;
  ddp = (DIR *)buf;

  *ino = ddp->inode; // current inode, 1st entry ALWAYS
  
  cp += ddp->rec_len;
  ddp = (DIR *)cp;

  *p_ino = ddp->inode; // parent's inode, 2nd entry ALWAYS

  return 0;
}

int findname(INODE *parent, int ino_at, char name[])
{
  char buf[BLKSIZE], *cp, temp[256];
  DIR *dp;
  int i, ino;

  for(i = 0; i < 12 && parent->i_block[i]; i++) { // search all twelve direct blocks
    get_block(mp->dev, parent->i_block[i], buf);
    cp = buf;
    dp = (DIR *)buf;
    while(cp < buf + BLKSIZE) {
      if (ino_at == dp->inode && dp->name[0] != '.') {
        strncpy(name, dp->name, dp->name_len);
        name[dp->name_len] = 0;
        return 0;
      }

      cp += dp->rec_len;
      dp = (DIR *)cp;
    }
  }

  return -1;
}

MINODE *iget(int dev, int ino)
{
  int i;
  
  for(i = 0; i < NMINODES; i++) {
    if (minode[i].ref_count > 0 &&
	minode[i].dev == dev    && 
	minode[i].ino == ino) {
      // An INODE for ino already exists
      minode[i].ref_count++;
      if(DEBUGGING) {
	printf("..........................................\n");
	printf("iget ... found inode[%d, %d] in memory.\n", dev, ino);
	printf(" -> with name \"%s\"\n", minode[i].name);
	printf("..........................................\n");
      }
      return &(minode[i]);
    }
  }

  /* INODE does not exist in memory yet */
  MINODE *mip;
  int blk, offset;
  int p_ino = 0;
  char buf[BLKSIZE];

  i = 0;
  while(minode[i].ref_count > 0)
    i++;
  mip = &minode[i]; // get an inode with no refereces to it

  blk = (ino - 1) / INODES_PER_BLOCK + mp->iblock;
  offset = (ino - 1) % INODES_PER_BLOCK;

  get_block(mp->dev, blk, buf);
  ip = (INODE *)buf + offset;
  
  memcpy(&mip->Inode, ip, sizeof(INODE));
  
  mip->dev = dev;
  mip->ino = ino;
  mip->ref_count = 1;

  if(DEBUGGING) {
    printf("..........................................\n");
    printf("iget ... inode found [%d, %d] at block #%d\n", 
	   dev, ino, blk);
  }

  
  /* ------ GET THE FILE'S NAME ---------- */
  if(!S_ISDIR(mip->Inode.i_mode)) // dont continue if not a directory
    return mip;

  findino(mip, &ino, &p_ino);    // get the parent's ino

  if (ino == 2 && p_ino == 2) // ignore when initializing root
    return mip;

  blk = (p_ino - 1) / INODES_PER_BLOCK + mp->iblock;
  offset = (p_ino - 1) % INODES_PER_BLOCK;

  get_block(mp->dev, blk, buf);
  ip = (INODE *)buf + offset;    // Contains the parent inode

  findname(ip, ino, fname);
  strcpy(mip->name, fname);      // copy the found name into the mip's name
  if (DEBUGGING) {
    printf("  with name \"%s\", and parent inode %d\n", mip->name, p_ino);
    printf("    *parent ino = 0 means new inode\n");
    printf("..........................................\n");
  }
  /* -------------------------------------*/
  
  return mip;
}

void iput(MINODE *mip)
{
  int blk, offset;
  char buf[BLKSIZE];
  INODE *ip;

  //if(mip == root) {
  //  if (DEBUGGING) 
  //    printf("iput ... putting root.\n");
  //  return;
  //}
  mip->ref_count--;

  if(DEBUGGING) {
    printf("..............DEBUG................\n");
    printf("iput ... mip(\"%s\") ref_count = %d\n", mip->name, mip->ref_count);
    printf("Place mip back? ... ");
  }
  if (mip->ref_count > 0) { // do not remove inode if still in use elsewhere
    if (DEBUGGING) {
      printf("No, referenced elsewhere.\n");
      printf("...................................\n");
    }
    return;
  }
  if (mip->dirty == FALSE) { // no need to write back
    if(DEBUGGING) {
      printf("No, not dirty.\n");
      printf("...................................\n");
    }
    return;
  }

  if(DEBUGGING) {
    printf("Yes.\n");
    printf("...................................\n");
  }
  
  blk = (mip->ino - 1) / INODES_PER_BLOCK + mp->iblock;
  offset = (mip->ino - 1) % INODES_PER_BLOCK;
  get_block(mip->dev, blk, buf);

  ip = (INODE *)buf + offset;
  memcpy(ip, &mip->Inode, sizeof(INODE));
  put_block(mp->dev, blk, buf); 
}


int tokenize(char *path, char *delim)
{
  int numParts = 0, i = 1;
  char *temp, pathHolder[1024], c;

  bzero(pathHolder, 1024);

  strcpy(pathHolder, path);
  c = pathHolder[0];
  
  while(c) {
    if(c == delim[0])
      numParts++;
    c = pathHolder[i];
    i++;
  }
  numParts++;
  
  _free();

  if(strcmp(pathHolder, "/") == 0) {
    out = 0;
    return 0;
  }

  out = malloc((numParts + 1) * sizeof(char *));

  temp = strtok(pathHolder, delim);

  i = 0;
  while(temp) {
    out[i] = malloc(strlen(temp) + 1);
    strcpy(out[i], temp);
    out[i][strlen(temp)] = 0;

    temp = strtok(0, delim);
    i++;
  }
  out[i] = 0; // null the list

  return numParts;
}

int findCmd(char *command)
{
  int i = 0;

  for(i = 0; cmds[i] != 0; i++)
    {
      if (strcmp(command, cmds[i]) == 0) {
	//printf("Command: \'%s\'\n", cmds[i]);
	return i;
      }
    }

  return -1;
}


int search(MINODE *ip, char *name)
{
  char buf[BLKSIZE], *cp;
  char temp[EXT2_NAME_LEN];
  int i;

  for(i = 0; i < 12 && ip->Inode.i_block[i]; i++) {
    get_block(ip->dev, ip->Inode.i_block[i], buf);
    cp = buf;
    dp = (DIR *)buf;
    while(cp < buf + BLKSIZE) {
      bzero(temp, EXT2_NAME_LEN);
      strncpy(temp, dp->name, dp->name_len);
    
      if(strcmp(temp, name) == 0)
	return dp->inode;

      cp += dp->rec_len;
      dp = (DIR *)cp;
    }
  }

  return -1;
}

map(char buf[], int len)
{
  int  i;

  for (i=0; i < len; i++){
    //(tst_bit(buf, i)) ?	putchar('1') : putchar('0');
    if (i && (i % 8)==0)
       printf(" ");
    if (i && (i % 32) == 0)
      putchar('\n');
    (test_bit(buf, i)) ? putchar('1') : putchar('0');
  }
  printf("\n");
}

/* BITWISE MANIPULATION */
int test_bit(char buf[], int bit)
{
  int i = bit / 8;
  int j = bit % 8;
  
  if (buf[i] & (1 << j))
    return 1;
  return 0;
}

void set_bit(char buf[], int bit)
{
  int i = bit / 8;
  int j = bit % 8;

  buf[i] |= (1 << j);
}

void clear_bit(char buf[], int bit)
{
  int i = bit / 8;
  int j = bit % 8;

  buf[i] &= ~(1 << j);
}
/*********************************/  

void decFree(bool inode)
{
  char buf[BLKSIZE];

  /* decrement super block free inode count */
  get_block(mp->dev, 1, buf);
  sp = (SUPER *)buf;
  if(inode)
    sp->s_free_inodes_count--;
  else
    sp->s_free_blocks_count--;
  put_block(mp->dev, 1, buf);

  int gp_blk = sp->s_first_data_block + 1;

  /* decrement group desc free inode count */
  get_block(mp->dev, 2, buf);
  gp = (GD *)buf;
  if(inode)
    gp->bg_free_inodes_count--;
  else
    gp->bg_free_blocks_count--;
  put_block(mp->dev, gp_blk, buf);
}

int ialloc()
{
  int i;
  char buf[BLKSIZE];

  get_block(mp->dev, mp->imap, buf); // read the inode bitmap for this device

  // Test all bits until a free one is found
  for (i = 0; i < mp->ninodes; i++) { 
    if(test_bit(buf, i) == 0) { // A free inode is found, set it
      set_bit(buf, i);
      decFree(TRUE);
      put_block(mp->dev, mp->imap, buf);

      if(DEBUGGING) {
	printf("ialloc ->> [%d, %d]\n", mp->dev, i + 1);
      }

      return i + 1; // return ino of the allocated block
    }
  }

  if(DEBUGGING) {
    printf("ialloc ->> No free inodes.\n");
  }

  return 0; // no free inodes left
}

int _balloc()
{
  int i = 0;
  char buf[BLKSIZE];

  get_block(mp->dev, mp->bmap, buf); // read the block bitmap for this device

  // Test all bits until a free one is found
  while (i < mp->nblocks) { 
    //printf("i = %d\n", i);
    if(test_bit(buf, i) == 0) { // A free block is found, set it
      set_bit(buf, i);
      decFree(FALSE);
      put_block(mp->dev, mp->bmap, buf);
      
      if(DEBUGGING) {
	printf("balloc ->> new data block at #%d\n", i + 1);
      }

      return i + 1; // return bno of the allocated block
    }

    i++;
  }

  if(DEBUGGING) {
    printf("balloc ->> No free blocks.\n");
  }

  return 0; // no free blocks left
}

void _free()
{
  int j = 0;

  if (out) { // free out
    while(j < numTokens) {
      free(out[j]);
      j++;
    }
    free(out);
  }
}


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

  if(numTokens == 0) // no tokens
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

int enter_name(MINODE *pip, int n_ino, char *name, int type)
{
  char buf[BLKSIZE], *cp;
  int Ideal_Len, n_name_len, remaining, needed;
  int i = 0;

  n_name_len = strlen(name);             // new name's length
  needed = 4 * ((8 + n_name_len + 3) / 4 );  // needed record length

  // Get the last active block
  while(pip->Inode.i_block[i + 1]) 
    i++;

  get_block(mp->dev, pip->Inode.i_block[i], buf);
  cp = buf;
  dp = (DIR *)buf;
  
  // Get the last record item
  while(cp < buf + BLKSIZE) {
    if(cp + dp->rec_len  == buf + BLKSIZE)
      break;

    cp += dp->rec_len;
    dp = (DIR *)cp;
  }

  Ideal_Len = 4 * ((8 + dp->name_len + 3) / 4 );  // last record's ideal length
  remaining = dp->rec_len - Ideal_Len;

  if (DEBUGGING) {
    char temp[256];
    strncpy(temp, dp->name, dp->name_len);
    temp[dp->name_len] = 0;
    printf("Entering new name in parent inode:\n");
    printf("  Name ......................... %s\n", name);
    printf("  Name Length .................. %d\n", n_name_len);
    printf("  Needed Rec Length ............ %d\n", needed);
    printf("  Last Record before new ....... \"%s\"\n", temp);
    printf("  Last Records Ideal Length .... %d\n", Ideal_Len);
    printf("  Remaining Record Length ...... %d\n", remaining);
  }

  if (remaining >= needed) {
    dp->rec_len = Ideal_Len;
    cp += dp->rec_len;
    dp = (DIR *)cp;

    dp->inode     = n_ino;
    dp->rec_len   = remaining;
    dp->name_len  = n_name_len;
    dp->file_type = type;
    strncpy(dp->name, name, dp->name_len);
  }
  else {
    // TODO : If no room left of block
  }

  put_block(mp->dev, pip->Inode.i_block[i], buf); // THIS WILL CHANGE WHEN TODO IS ENTERED
}

