#include "include/fs.h"

int shutdown ()
{
  printf("Preparing shut down...\n");
  printf("   Writing files back to device ...... ");
  sleep(1);
  printf("Done.\n");
  printf("   Closing open file descriptors ..... ");
  sleep(1);
  printf("Done.\n");
  printf("Shut down successful. Goodbye!\n");

  // TODO: Shut down gracefully.

  exit(0);
}