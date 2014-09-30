#ifndef FRAC_H
#define FRAC_H

#define X_TRANS 0.5

#define bool int
#define true 1
#define false 0

//this structure contains the parameters for the fractal generating threads
typedef struct
{
  int shift;    // number of thread minus one
  int step;     // number of threads
} generateThreadArg;




extern float xpos;
extern float ypos;
extern float zoom2;

// complex parameter for the chosen fractal
extern float gcx, gcy;


bool isOK_Mandelbrot(int xx, int yy, int* outside);
bool isOK_ConjMandelbrot(int xx, int yy, int* outside);
bool isOK_Cubic(int xx, int yy, int* outside);
bool isOK_ConjCubic(int xx, int yy, int* outside);
bool isOK_Quartic(int xx, int yy, int* outside);
bool isOK_ConjQuartic(int xx, int yy, int* outside);
bool isOK_Julia(int xx, int yy, int* outside);

int getFileNumber();

#endif

