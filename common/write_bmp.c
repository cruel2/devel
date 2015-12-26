
/*
    write_bmp.c
    
    Includes write_bmp()
*/

#include <stdio.h>
#include <stdlib.h>


/*
    Writes memory bitmap to BMP file
    c        : pointer to memory area
    width    : width of bitmap
    height   : height of bitmap
    filename : name of file
*/
int write_bmp(unsigned char* p, int width, int height, char* filename)
{
  FILE* f;
  int i;   /*  for iteration  */
  
  unsigned char bmp_header[14] = {
      'B', 'M', 0, 0, 0,
      0  , 0  , 0, 0, 0,
      54,  0,   0, 0
  };
  
  unsigned char bmp_infoheader[40] = {
      40, 0, 0, 0, 0,
      0,  0, 0, 0, 0,
      0,  0, 1, 0, 24,
      0
  };
  
  unsigned char bmp_pad[3] = {
      0, 0, 0
  };
  
  int size = 54 + 3 * width * height;
  
  /*  serialize bitmap size  */
  bmp_header[2] = (unsigned char) size;
  bmp_header[3] = (unsigned char) (size >> 8);
  bmp_header[4] = (unsigned char) (size >> 16);
  bmp_header[5] = (unsigned char) (size >> 24);
  
  /*  serialize bitmap width  */
  bmp_infoheader[4] = (unsigned char) width;
  bmp_infoheader[5] = (unsigned char) (width >> 8);
  bmp_infoheader[6] = (unsigned char) (width >> 16);
  bmp_infoheader[7] = (unsigned char) (width >> 24);
  
  /*  serialize bitmap height  */
  bmp_infoheader[8]  = (unsigned char) height;
  bmp_infoheader[9]  = (unsigned char) (height >> 8);
  bmp_infoheader[10] = (unsigned char) (height >> 16);
  bmp_infoheader[11] = (unsigned char) (height >> 24);
  
  f = fopen(filename, "wb");
  fwrite(bmp_header, 1, 14, f);
  fwrite(bmp_infoheader, 1, 40, f);
  
  for (i = 0; i < height; i++)
  {
    fwrite(p + (width * (height - i -1) * 3), 3, width, f);
    fwrite(bmp_pad, 1, (4 - (width * 3) % 4) % 4, f);
  }
  
  fclose(f); 
}

