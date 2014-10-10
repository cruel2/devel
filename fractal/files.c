
#include <dirent.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "config.h"
#include "frac.h"


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


void writeBmp()
{
  char fileName[13];
  char* myBitmap;
  char* oldBitmap;
  int myFileSize;
  int oldWidth, oldHeight;
  
  if (PRINTDOUBLE)
  {
    oldBitmap = bitmap;
    myBitmap = (char*) malloc (HEIGHT*WIDTH*4*4);
    bitmap = myBitmap;
    generate();
  }
  else
    myBitmap = bitmap;
  
  fileNumber++;
  strncpy(fileName, "test", 4);
  sprintf(fileName + 4, "%4.4d", fileNumber);
  strncpy(fileName + 8, ".bmp", 4);
  
  oldWidth = WIDTH;
  oldHeight = HEIGHT;
  
  if (PRINTDOUBLE)
  {
    WIDTH *= 2;
    HEIGHT *= 2;
  }
  
  myFileSize = WIDTH * HEIGHT * 4;
      
  FILE *fp = fopen(fileName,"wb");
  bitmap_type *pbitmap  = (bitmap_type*)calloc(1,sizeof(bitmap_type));
  uint8_t *pixelbuffer = (uint8_t*)(myBitmap);
  pbitmap->fileheader.signature[0] = 'B';
  pbitmap->fileheader.signature[1] = 'M';
  pbitmap->fileheader.filesize = myFileSize;
  pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap_type);
  pbitmap->bitmapinfoheader.dibheadersize = sizeof(bitmapinfoheader_type);
  pbitmap->bitmapinfoheader.width = WIDTH;
  pbitmap->bitmapinfoheader.height = HEIGHT;
  pbitmap->bitmapinfoheader.planes = _planes;
  pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
  pbitmap->bitmapinfoheader.compression = _compression;
  pbitmap->bitmapinfoheader.imagesize = myFileSize;
  pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter ;
  pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter ;
  pbitmap->bitmapinfoheader.numcolorspallette = 0;
  fwrite (pbitmap, 1, sizeof(bitmap_type),fp);
  
  fwrite(pixelbuffer, 1, myFileSize, fp);
  fclose(fp);
  free(pbitmap);
  
  if (PRINTDOUBLE)
  {
    WIDTH /= 2;
    HEIGHT /= 2;
    myFileSize = WIDTH * HEIGHT * 4;
    bitmap = oldBitmap;
    free(myBitmap);
  }
}

