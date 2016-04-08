#include "../include/fs.h"

map(char buf[], int len)
{
  int  i;

  for (i=0; i < len; i++){
    //(tst_bit(buf, i)) ? putchar('1') : putchar('0');
    if (i && (i % 8)==0)
       printf(" ");
    if (i && (i % 32) == 0)
      putchar('\n');
    (test_bit(buf, i)) ? putchar('1') : putchar('0');
  }
  printf("\n");
}