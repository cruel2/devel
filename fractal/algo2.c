#include "frac.h"

bool isOK_Mandelbrot(int xx, int yy, int* outside)
{
    double cx = xx * zoom2 + xpos - X_TRANS;
    double cy = yy * zoom2 + ypos;
    double x = cx;
    double y = cy;
    
    for (int i = 0; i < 100; i++)
    {
      if (x*x + y*y > 4.0)
      {
        *outside = i;
        return false;
      }
      
      double tx = x*x - y*y + cx;
      double ty = 2*x*y + cy;
      
      x = tx;
      y = ty;
    }
    
    *outside = 0;
    return true;
}


bool isOK_ConjMandelbrot(int xx, int yy, int* outside)
{
    double cx = xx * zoom2 + xpos - X_TRANS;
    double cy = yy * zoom2 + ypos;
    double x = cx;
    double y = cy;
    
    for (int i = 0; i < 100; i++)
    {
      if (x*x + y*y > 4.0)
      {
        *outside = i;
        return false;
      }
      
      double tx = x*x - y*y + cx;
      double ty = -2*x*y + cy;
      
      x = tx;
      y = ty;
    }
    
    *outside = 0;
    return true;
}


bool isOK_Cubic(int xx, int yy, int* outside)
{
    double cx = xx * zoom2 + xpos - X_TRANS;
    double cy = yy * zoom2 + ypos;
    double x = cx;
    double y = cy;
    
    for (int i = 0; i < 100; i++)
    {
      if (x*x + y*y > 4.0)
      {
        *outside = i;
        return false;
      }
      
      double tx = x*x*x - 3*x*y*y + cx;
      double ty = 3*x*x*y - y*y*y + cy;
      
      x = tx;
      y = ty;
    }
    
    *outside = 0;   
    return true;
}


bool isOK_ConjCubic(int xx, int yy, int* outside)
{
    double cx = xx * zoom2 + xpos - X_TRANS;
    double cy = yy * zoom2 + ypos;
    double x = cx;
    double y = cy;
    
    for (int i = 0; i < 100; i++)
    {
      if (x*x + y*y > 4.0)
      {
        *outside = i;
        return false;
      }
      
      double tx = x*x*x - 3*x*y*y + cx;
      double ty = - 3*x*x*y + y*y*y + cy;
      
      x = tx;
      y = ty;
    }
    
    *outside = 0;
    return true;
}    


bool isOK_Quartic(int xx, int yy, int* outside)
{
    double cx = xx * zoom2 + xpos - X_TRANS;
    double cy = yy * zoom2 + ypos;
    double x = cx;
    double y = cy;
    
    for (int i = 0; i < 100; i++)
    {
      if (x*x + y*y > 4.0)
      {
        *outside = i;
        return false;
      }
      
      double tx = (x*x - y*y) * (x*x - y*y) - 4*x*x*y*y + cx;
      double ty = 4*x*y*(x*x - y*y) + cy;
      
      x = tx;
      y = ty;
    }
    
    *outside = 0;
    return true;
}


bool isOK_ConjQuartic(int xx, int yy, int* outside)
{
    double cx = xx * zoom2 + xpos - X_TRANS;
    double cy = yy * zoom2 + ypos;
    double x = cx;
    double y = cy;
    
    for (int i = 0; i < 100; i++)
    {
      if (x*x + y*y > 4.0)
      {
        *outside = i;
        return false;
      }
      
      double tx = (x*x - y*y) * (x*x - y*y) - 4*x*x*y*y + cx;
      double ty = -4*x*y*(x*x - y*y) + cy;
      
      x = tx;
      y = ty;
    }
    
    *outside = 0;
    return true;
}

