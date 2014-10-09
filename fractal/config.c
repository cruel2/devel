
#include "config.h"

int WIDTH;
int HEIGHT;
int BITSPERPIXEL;
int PIXELBYTESIZE;
int FILESIZE;
bool WRITECOORDS;
int STEPSIZE1;
int STEPSIZE2;
bool HYPHENNEEDED;
bool PRINTDOUBLE;
int NOTHREADS;


int readConfig()
{
  int temp = 0;
  FILE *fp = fopen("config.ini", "r");
  if (!fp)
  {
    printf("Can't open config file!\n");
    return -1;
  }
  
  if (!fscanf(fp, "WIDTH = %d\n", &WIDTH))
  {
    printf("Couldn't read width!\n");
    return -2;
  }
    
  if (!fscanf(fp, "HEIGHT = %d\n", &HEIGHT))
  {
    printf("Couldn't read height!\n");
    return -2;
  }
  
  if (!fscanf(fp, "BITSPERPIXEL = %d\n", &temp))
  {
    printf("Couldn't read bitsperpixel!\n");
    return -2;
  }
  
  if (!fscanf(fp, "WRITECOORDS = %d\n", &temp))
  {
    printf("Couldn't read write coords!\n");
    return -2;
  }
  if (temp == 1)
    WRITECOORDS = true;
  else if (temp == 0)
    WRITECOORDS = false;
  else
  {
    printf("Invalid value for write coords!\n");
    return -2;
  }
  
  if (!fscanf(fp, "STEPSIZE1 = %d\n", &STEPSIZE1))
  {
    printf("Couldn't read step size 1!\n");
    return -2;
  }

  if (!fscanf(fp, "STEPSIZE2 = %d\n", &STEPSIZE2))
  {
    printf("Couldn't read step size 2!\n");
    return -2;
  }
  
  if (!fscanf(fp, "HYPHENNEEDED = %d\n", &temp))
  {
    printf("Couldn't read hyphen needed!\n");
    return -2;
  }
  
  if (temp == 1)
    HYPHENNEEDED = true;
  else if (temp == 0)
    HYPHENNEEDED = false;
  else
  {
    printf("Invalid value for hyphen needed!\n");
    return -2;
  }
  
  if (!fscanf(fp, "PRINTDOUBLE = %d\n", &temp))
  {
    printf("Couldn't read print double!\n");
    return -2;
  }
  
  if (temp == 1)
    PRINTDOUBLE = true;
  else if (temp == 0)
    PRINTDOUBLE = false;
  else
  {
    printf("Invalid value for print double!\n");
    return -2;
  }
  
  if (!fscanf(fp, "NOTHREADS = %d\n", &NOTHREADS))
  {
    printf("Couldn't read number of threads!\n");
    return -2;
  }
  
  PIXELBYTESIZE = WIDTH * HEIGHT * _bitsperpixel / 8;
  FILESIZE = PIXELBYTESIZE + sizeof(WIDTH*HEIGHT*4);  // TODO ez igy biztos, hogy jo?
  
  // only for debug
  if (0)
  {
    printf("Config file reading - DEBUG\n");
    printf("WIDTH:        %d\n", WIDTH);
    printf("HEIGHT:       %d\n", HEIGHT);
    printf("BITSPERPIXEL: %d\n", BITSPERPIXEL);
    printf("WRITECOORDS:  %d\n", (int)WRITECOORDS);
    printf("STEPSIZE1:    %d\n", STEPSIZE1);
    printf("STEPSIZE2:    %d\n", STEPSIZE2);
    printf("HYPHENNEEDED: %d\n", (int)HYPHENNEEDED);
    printf("PRINTDOUBLE:  %d\n", (int)PRINTDOUBLE);
    printf("NOTHREADS:    %d\n", NOTHREADS);
  }
  
  return 1;
}

