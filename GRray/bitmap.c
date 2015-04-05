
#include <stdlib.h>
#include <stdio.h>

#include "particle.h"


char* bmpFile;


void readBmp()
{
  long readBytes = 0;
  
  bmpFile = (char*)malloc(1500000);
  if (bmpFile == NULL)
  {
    fprintf(stderr, "Memoria allokacio nem sikerult.\n");
    exit(-1);
  }
  
  FILE* fp = fopen("test.bmp", "rb");
  
  readBytes = fread(bmpFile, 1, 1500000, fp);
  
  fclose(fp);  
}

void destroyBmp()
{
  free(bmpFile);
}

