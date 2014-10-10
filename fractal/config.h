
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

#include "frac.h"

#define _bitsperpixel 32


// special settings for certain fractal types
// These settings will be overwritten by command-line parameters.
typedef struct
{
  float xpos;
  float ypos;
  float zoom;
  int noIterations;  
} fractalConfig_type;


extern int WIDTH;
extern int HEIGHT;
extern int BITSPERPIXEL;
extern int PIXELBYTESIZE;
extern int FILESIZE;
extern bool WRITECOORDS;
extern int STEPSIZE1;
extern int STEPSIZE2;
extern bool HYPHENNEEDED;
extern bool PRINTDOUBLE;
extern int NOTHREADS;
extern int NOITERATIONS;
extern bool READONLY;


int readConfig();
int writeConfig();


#endif

