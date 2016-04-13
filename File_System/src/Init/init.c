#include "../include/fs.h"

int mount_root();

int init()
{
  int i, j;
  PROC *proc_ptr;

  for(i = 0; i < NMINODES; i++)
    minode[i].ref_count = 0;
  for(i = 0; i < NMOUNT; i++)
    mounttab[i].busy = FALSE;
  for(i = 0; i < NPROC; i++) {
    proc[i].status = FREE;
    for (j = 0; j < NFD; j++)
      proc[i].fd[j] = 0;
    proc[i].next = &proc[i + 1];
  }
  for(i = 0; i < NOFT; i++)
    oft[i].ref_count = 0;
  
  mount_root();

  printf("Creating Processes : P0, P1\n");

  /* P0 */
  proc_ptr = running = &proc[0];
  proc_ptr->status = BUSY;
  proc_ptr->uid = proc_ptr->pid = proc_ptr->ppid = proc_ptr->gid = 0;
  proc_ptr->parent = proc_ptr->sibling = proc_ptr;
  proc_ptr->child = 0;
  proc_ptr->cwd = root;
  proc_ptr->cwd->ref_count++;
  /* END P0 */

  /* P1 */
  proc_ptr = &proc[1];
  proc_ptr->next = &proc[0];
  proc_ptr->status = BUSY;
  proc_ptr->uid = 2;
  proc_ptr->pid = 1;
  proc_ptr->ppid = proc_ptr->gid = 0;
  proc_ptr->cwd = root;
  proc_ptr->cwd->ref_count++;

  nproc = 2;
  
  printf("Initialization Successful.\n\n");
}
