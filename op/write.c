#include "../include/fs.h"

static int buf_size = 1024;

int _write ()
{
  char buf[buf_size];
  int fd = -1, nbytes = -1;

  if (!out[1])
  {
    printf("Usage: read FD N_BYTES\n");
    return -1;
  }
  if (!out[2])
  {
    printf("Missing argument #2.\n");
    return -2;
  }

  if (strcmp(out[1], "0") == 0) // atoi cannot convert string "0" to int 0
  {
    fd = 0;
  }
  else
  {
    fd = atoi(out[1]);
    if (fd == 0)
    {
      printf("\"%s\" : not an integer value. (Valid: 0,1,2,3...)\n", out[1]);
      return -2;
    }

    if (fd >= NFD)
    {
      printf("\"%d\" : Not within specified range. (Valid: 0-%d)\n", fd, NFD - 1);
      return -3;
    }
  }

  bzero(buf, buf_size);
  nbytes = strlen(out[2]);
  if (nbytes > (buf_size - 1))
  {
    printf("Buffer not large enough; try writing in increments.\n");
    return -4;
  }

  strncpy(buf, out[2], nbytes);    // Copy the message into the buffer space

  return __write(fd, buf, nbytes); // Write to stdout
}