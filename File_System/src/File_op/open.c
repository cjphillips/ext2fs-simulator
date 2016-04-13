#include "../include/fs.h"

int _open ()
{
  int mode = 0;
  char file_name[INODE_NAME];

  if (!out[1])
  {
    printf("No target file specified.\n");
    return -1;
  }
  if (out[2]) // mode has been specified - default is 0 (RD)
  {
    switch(atoi(out[2]))
    {
      case 0: break;              // READ
      case 1: mode = 1; break;    // WRITE
      case 2: mode = 2; break;    // READ-WRITE
      case 3: mode = 3; break;    // APPEND
      default:  
        printf ("Invalid mode. (Valid: 0-3)\n");
        return -2;
    }
  }

  strcpy(file_name, out[1]);

  return __open(file_name, mode);
}