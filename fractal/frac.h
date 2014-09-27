#ifndef FRAC_H
#define FRAC_H

#define X_TRANS 0.5


//this structure contains the parameters for the fractal generating threads
struct generateThreadArg
{
  int shift;    // number of thread minus one
  int step;     // number of threads
};


extern float xpos;
extern float ypos;
extern float zoom2;


bool isOK_Mandelbrot(int xx, int yy, int* outside);
bool isOK_ConjMandelbrot(int xx, int yy, int* outside);
bool isOK_Cubic(int xx, int yy, int* outside);
bool isOK_ConjCubic(int xx, int yy, int* outside);
bool isOK_Quartic(int xx, int yy, int* outside);
bool isOK_ConjQuartic(int xx, int yy, int* outside);

#endif

