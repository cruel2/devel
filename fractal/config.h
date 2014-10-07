
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

#include "frac.h"

#define _bitsperpixel 32
#define PICS 450


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


int readConfig();


#endif

