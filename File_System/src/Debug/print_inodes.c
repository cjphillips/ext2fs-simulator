#include "../include/fs.h"

int print_inodes()
{
  int i = 0;

  printf("In Memory Inodes:\n");
  printf("ino  Dev  # Ref  Dirty  Mounted\n");
  printf("---  ---  -----  -----  -------\n");

  while(i < NMINODES)
  {
    if (minode[i].ref_count > 0)
    {
      printf("%3d", minode[i].ino);
      printf("  %3d", minode[i].dev);
      printf("     %2d", minode[i].ref_count);

      switch(minode[i].dirty)
      {
        case TRUE:   printf("      Y"); break;
        case FALSE:  printf("      N"); break;
        default:     printf("      N"); break;
      }
      switch(minode[i].mounted)
      {
        case TRUE:   printf("        Y\n"); break;
        case FALSE:  printf("        N\n"); break;
        default:     printf("        N\n"); break;
      }
    }

    i++;
  }
}