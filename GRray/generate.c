
#include <math.h>
#include <stdio.h>
#include "particle.h"


int getColor(double x, double y, double z, char* color)
{
  if (z > OBJECTZ)
  {
    int xint = (int) (x * 4.0 + 1000);
    int yint = (int) (y * 4.0 + 1000);
    
    if ((xint % 2 == 1 && yint % 2 == 0) || (xint % 2 == 0 && yint % 2 == 1))
      *color = 200;
    else
      *color = 0;
    return 1;
  }
  
  return 0;
}


int getColor2(double x, double y, double z, RGBColor* rgb)
{
  if (z > OBJECTZ)
  {
    // x <-> y (because of OpenGL?)
    int xint = (int) (y * 600.0f + 250.0f);
    int yint = (int) (x * 600.0f + 150.0f);
    
    if (xint >=0 && xint < BMPSIZE && yint >= 0 && yint < BMPSIZE)
    {
      rgb->r =
         bmpFile[xint * 3 + yint * BMPROW + 2 + BMPOFFSET];
      rgb->g =
         bmpFile[xint * 3 + yint * BMPROW + 1 + BMPOFFSET];
      rgb->b =
         bmpFile[xint * 3 + yint * BMPROW + 0 + BMPOFFSET];
    }
    else
    {
      rgb->r = 0;
      rgb->g = 0;
      rgb->b = 0;
    }
    return 1;
  }
  
  return 0;
}


void generate()
{
  int i, j, k;
  
  RGBColor rgb;
  
  for (i = 0; i < HEIGHT; i++)
  {
    if (i % 40 == 0)
      printf("%d. sor\n", i);
      
    for (j = 0; j < WIDTH; j++)
    {
      bitmap[(i * WIDTH + j) * 4 + 0] = 0;
      bitmap[(i * WIDTH + j) * 4 + 1] = 0;
      bitmap[(i * WIDTH + j) * 4 + 2] = 0;
        
      particle s;
      
      s.x = (i - HEIGHT / 2) * pixelSize;
      s.y = (j - WIDTH  / 2) * pixelSize;
      s.z = DEPTH;
      
      Cart2Sphe(&s);
            
      float length = sqrt(s.x*s.x + s.y*s.y + s.z*s.z);
      
      s.vx = s.x / length * C;
      s.vy = s.y / length * C;
      s.vz = s.z / length * C;
      
      
      s.vt = C;
      
      
      //char color;
      
      for (k = 0; k < 1000; k++)
      {
        if (getColor2(s.x, s.y, s.z, &rgb) == 1)
        {
          bitmap[(i * WIDTH + j) * 4 + 0] = rgb.r;
          bitmap[(i * WIDTH + j) * 4 + 1] = rgb.g;
          bitmap[(i * WIDTH + j) * 4 + 2] = rgb.b;
          break;
        }

        step_cartesian(&s);        
        
        if (s.r <= rg)
          break;
        
      }
    }
  }
}

