#include "../include/fs.h"

int menu ()
{ 
  printf("<-----------------------------Commands Menu--->\n");
  printf("  Basic Operations:\n");
  printf("       ls [path]\n");
  printf("       cd [path]\n");
  printf("      pwd\n");
  printf("    mkdir  path\n");
  printf("    rmdir  path\n\n");
  printf("    chgrp  grp  path\n");
  printf("    chown  user path\n");
  printf("    chmod  perm path\n\n");
  printf("    creat  path\n");
  printf("       rm  path\n\n");

  printf("  File Operations:\n");
  printf("     link  file link_name\n");
  printf("  symlink  file link_name\n");
  printf(" readlink  link\n");
  printf("     stat  file\n");
  printf("    touch  file\n");;
  printf("     open  file [mode]\n");
  printf("    close  file_desc\n");
  printf("      cat  file\n");
  printf("       cp  src dest\n");
  printf("       mv  src dest\n");
  printf("     read  file_desc\n");
  printf("    write  file_desc\n");
  printf("     poft\n\n");
  printf("  Mounting:\n");
  printf("    mount  dev\n");
  printf("   umount  dev\n");
  printf("<--------------------------------------------->\n");
}