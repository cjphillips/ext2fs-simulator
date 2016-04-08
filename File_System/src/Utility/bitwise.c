#include "../include/fs.h"

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