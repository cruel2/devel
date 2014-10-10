
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
int NOITERATIONS;
bool READONLY;


int readConfig()
{
  int value;
  char name[20];
  
  FILE *fp = fopen("config.ini", "r");
  
  if (!fp)
  {
    printf("Can't open config file!\n");
    return -1;
  }
  
  while(fscanf(fp, "%s = %d\n", &name[0], &value) != EOF)
  {
    if (strncmp("WIDTH", &name[0], 6) == 0)
    {
      WIDTH = value;
      continue;
    }
      
    if (strncmp("HEIGHT", &name[0], 6) == 0)
    {
      HEIGHT = value;
      continue;
    }
    
    if (strncmp("BITSPERPIXEL", &name[0], 12) == 0)
    {
      BITSPERPIXEL = value;
      continue;
    }
    
    if (strncmp("WRITECOORDS", &name[0], 11) == 0)
    {
      if (value == 1)
        WRITECOORDS = true;
      else if (value == 0)
        WRITECOORDS = false;
      else
      {
        printf("Invalid value for write coords!\n");
        return -2;
      }

      continue;
    }
    
    if (strncmp("STEPSIZE1", &name[0], 9) == 0)
    {
      STEPSIZE1 = value;
      continue;
    }
    
    if (strncmp("STEPSIZE2", &name[0], 9) == 0)
    {
      STEPSIZE2 = value;
      continue;
    }
    
    if (strncmp("HYPHENNEEDED", &name[0], 12) == 0)
    {
      if (value == 1)
        HYPHENNEEDED = true;
      else if (value == 0)
        HYPHENNEEDED = false;
      else
      {
        printf("Invalid value for hyphen needed!\n");
        return -2;
      }
  
      continue;
    }
    
    if (strncmp("PRINTDOUBLE", &name[0], 11) == 0)
    {
      if (value == 1)
        PRINTDOUBLE = true;
      else if (value == 0)
        PRINTDOUBLE = false;
      else
      {
        printf("Invalid value for print double!\n");
        return -2;
      }
      
      continue;
    }
    
    if (strncmp("NOTHREADS", &name[0], 9) == 0)
    {
      NOTHREADS = value;
      continue;
    }
    
    if (strncmp("NOITERATIONS", &name[0], 12) == 0)
    {
      NOITERATIONS = value;
      continue;
    }
    
    if (strncmp("READONLY", &name[0], 8) == 0)
    {
      if (value == 1)
        READONLY = true;
      else if (value == 0)
        READONLY = false;
      else
      {
        printf("Invalid value for read only!\n");
        return -2;
      }
      
      continue;
    }
  }
  
  PIXELBYTESIZE = WIDTH * HEIGHT * _bitsperpixel / 8;
  FILESIZE = PIXELBYTESIZE + sizeof(WIDTH*HEIGHT*4);  // TODO ez igy biztos, hogy jo?
  
  // only for debug
  if (1)
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
    printf("NOITERATIONS: %d\n", NOITERATIONS);
    printf("READONLY:     %d\n", READONLY);
  }
  
  fclose(fp);
  
  return 1;
}


int writeConfig()
{
  FILE* fp = fopen("config.ini", "w");
    
  fprintf(fp, "WIDTH = %d\n", WIDTH);
  fprintf(fp, "HEIGHT = %d\n", HEIGHT);
  fprintf(fp, "BITSPERPIXEL = %d\n", BITSPERPIXEL);
  fprintf(fp, "WRITECOORDS = %d\n", WRITECOORDS);
  fprintf(fp, "STEPSIZE1 = %d\n", STEPSIZE1);
  fprintf(fp, "STEPSIZE2 = %d\n", STEPSIZE2);
  fprintf(fp, "HYPHENNEEDED = %d\n", HYPHENNEEDED);
  fprintf(fp, "PRINTDOUBLE = %d\n", PRINTDOUBLE);
  fprintf(fp, "NOTHREADS = %d\n", NOTHREADS);
  fprintf(fp, "NOITERATIONS = %d\n", NOITERATIONS);
  fprintf(fp, "READONLY = %d\n", READONLY);
  fprintf(fp, "\n");
  
  fclose(fp);
  
  return 1;
}

