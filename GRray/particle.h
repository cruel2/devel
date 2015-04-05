
#ifndef _PARTICLE_H
#define _PARTICLE_H


//#define DEBUG
#define pixelSize 0.00015
#define rg        0.005
#define CENTERZ   2.0
#define DEPTH     0.4
#define STEP      0.02
#define C         1.0f
#define OBJECTZ   4.0

#define HEIGHT    240        // 420p
#define WIDTH     426        // 420p
#define BMPROW    1848       // 615 x 348 test.bmp
#define BMPOFFSET 54
#define BMPSIZE   552
#define NO_BMPS   3


struct RGBColor_struct
{
  char r, g, b;
};

struct particle_struct
{
  float x, y, z;
  float r, theta, phy;
  
  float vx, vy, vz;
  float vr, vtheta, vphy;
  float vt;
};

typedef struct RGBColor_struct RGBColor;

typedef struct particle_struct particle;

extern char* bitmap;
extern char* bmpFile;
extern float CENTERX, CENTERY;


void Cart2Sphe(particle* s);
void generate();
void readBmp();
void step_cartesian(particle *s);

#endif

