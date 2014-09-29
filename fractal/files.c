#include <dirent.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getFileNumber()
{
  DIR* dir;
  
  int number;
  int reti;
  regex_t regex;
  char namebuf[100];
  
  strncpy(namebuf, "aaaaaaaaaa", 20);
  
  reti = regcomp(&regex, "test[0-9][0-9][0-9][0-9].bmp", 0);
  if (reti)
  {
    fprintf(stderr, "Could not compile regex!\n");
    return -1;
  }
  
  struct dirent *ent;
  if ((dir = opendir("./")) != NULL)
  {
    while ((ent = readdir(dir)) != NULL)
    {
      reti = regexec(&regex, ent->d_name, 0, NULL, 0);
      if (!reti)
      {
        if (strcmp(namebuf, ent->d_name) < 0)
          strncpy(namebuf, ent->d_name, 20);
      }
    }
    
    number = atoi(namebuf + 4);
    closedir(dir);
    
    if (number > 0)
      return number;
    
    if (strcmp(namebuf, "aaaaaaaaaa") == 0)
      return 0;
      
    return -1;
  }
  else
  {
    perror("");
    return -1;
  }
}
