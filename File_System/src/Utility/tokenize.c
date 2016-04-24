#include "../include/fs.h"

int tokenize(char *path, char *delim)
{
  int numParts = 0, i = 1, j = 0, quotedAt = -1;
  char *temp, pathHolder[1024], quoted[1024], c;

  bzero(pathHolder, 1024);
  bzero(quoted, 1024);

  strcpy(pathHolder, path);
  c = pathHolder[0];
  
  while(c) {
    if(c == '"')
    {
      quotedAt = numParts;
      while(pathHolder[i] && pathHolder[i] != '"')
      {
        quoted[j] = pathHolder[i];
        i++; j++;
      }
      quoted[j] = 0;
      i++;
      c = pathHolder[i];
    }

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
    if (i == quotedAt)
    {
      out[i] = malloc(strlen(quoted) + 1);
      strcpy(out[i], quoted);
      out[i][strlen(quoted)] = 0;
      if(i == numParts - 1)
      {
        i++;
        break;
      }
    }
    else
    {
      out[i] = malloc(strlen(temp) + 1);
      strcpy(out[i], temp);
      out[i][strlen(temp)] = 0;
      temp = strtok(0, delim);
    }
    i++;
  }
  out[i] = 0; // null the list

  return numParts;
}