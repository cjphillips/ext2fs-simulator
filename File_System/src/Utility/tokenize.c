#include "../include/fs.h"

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