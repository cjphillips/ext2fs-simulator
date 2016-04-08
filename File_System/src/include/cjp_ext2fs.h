#ifndef FS_H
#define FS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <libgen.h>

#include <errno.h>
#include <string.h>
#include <ext2fs/ext2_fs.h>

#define MAX 256

/*Defining short names for the EXT2 FS's structs*/
typedef struct ext2_super_block   SUPER;
typedef struct ext2_group_desc    GD;
typedef struct ext2_inode         INODE;
typedef struct ext2_dir_entry_2   DIR;

typedef enum boolean {
  FALSE, TRUE
} bool;

/* Block Info */
#define BLKSIZE           1024
#define BITS_PER_BLK      (8*BLKSIZE)
#define INODES_PER_BLOCK  (BLKSIZE/sizeof(INODE))

#define MAGIC_NUM         0xEF53

#define SUPERBLK          1

/* Various Table Sizes */ 
#define NMINODES          50
#define NMOUNT            10
#define NPROC             10
#define NFD               10
#define NOFT              50

/* Mount name lengths */
#define INODE_NAME        128
#define MOUNT_NAME        256
#define CJP_EXT2FS_LEN    256

/* PROC status */
#define FREE              0
#define BUSY              1
#define KILLED            2

typedef struct Oft
{
  int mode;
  int ref_count;
  struct Minode *inode_ptr;
  long offset;
} OFT;

/* Process Structure */
typedef struct Proc
{
  int uid;
  int pid;
  int gid;
  int ppid;
  int status;

  struct Minode *cwd;
  OFT    *fd[NFD];

  struct Proc *next;
  struct Proc *parent;
  struct Proc *child;
  struct Proc *sibling;
} PROC;

/* In-memory inode Structure */
typedef struct Minode
{
  INODE Inode;              // inode from disk
  int dev;
  int ino;
  
  int ref_count;
  enum boolean dirty;
  enum boolean mounted;
  struct Mount *mount_ptr;
  char name[INODE_NAME];           // name of file
} MINODE;

typedef struct Mount
{
  int ninodes;
  int nblocks;
  int bmap;
  int imap;
  int iblock;
  int dev;
  enum boolean busy;
  struct Minode *mounted_inode;
  char name[CJP_EXT2FS_LEN];
  char mount_name[MOUNT_NAME];
} MOUNT;

/* string array for tokenized parameter */
char **out;
int numTokens;

/*Defining Global pointers to specific blocks*/
GD     *gp;
SUPER  *sp;
INODE  *ip;
DIR    *dp;

MINODE *root;

char pathname[MAX], parameter[MAX], cwd[MAX];
char *root_dev;
int i_table_start;
int nproc;

int dev; // The device's file descriptor

MINODE minode[NMINODES];
MOUNT  mounttab[NMOUNT];
PROC   proc[NPROC], *running;
OFT    oft[NOFT];

int mount_root();

/*
void get_block(int fd, int blk, char buf[]);

int sb_info();

char get_blk_offset(int ino, int *blk);

void inode_info(INODE *at, char *name);

void show_blocks(INODE *iblk, int totalBlocks);

int decompose_path(char *path);

int search(char buf[], char *fname);
*/

#endif /* CJP_EXT2FS_H */
