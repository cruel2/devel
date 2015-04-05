
#include <math.h>
#include <stdio.h> // DEBUG

#include "particle.h"


float CENTERX;
float CENTERY;


void Cart2Sphe(particle* s)
{
  s->r = sqrt(s->x*s->x + s->y*s->y + (s->z-CENTERZ)*(s->z-CENTERZ));
  s->theta = atan((s->z-CENTERZ) / sqrt(s->x*s->x + s->y*s->y));
  s->phy = atan(s->y/s->x);

  s->vr = (s->vx*s->x + s->vy*s->y + s->vz*s->z) /
                 sqrt(s->x*s->x + s->y*s->y + (s->z-CENTERZ)*(s->z-CENTERZ));
                 
  s->vtheta = ((s->z-CENTERZ)*(s->x*s->vx + s->y*s->vy) - (s->x*s->x + s->y*s->y)*s->vz) /
                     (s->x*s->x+s->y*s->y+(s->z-CENTERZ)*(s->z-CENTERZ))*sqrt(s->x*s->x+s->y*s->y);
                     
  s->vphy = 0.0;
}


void step_spherical(particle *s)
{
  float g00r = rg / 2.0 / s->r / (s->r - rg);
  float grrr = -g00r;
  float gr00 = rg * (s->r - rg) / 2.0 / s->r / s->r / s->r;
  float grtt = rg - s->r;
  float grff = (rg - s->r) * sin(s->theta) * sin(s->theta);
  float gtrt = 1.0 / s->r;
  float gfrf = 1.0 / s->r;
  float gtff = -sin(s->theta) * cos(s->theta);
  float gfff = cos(s->theta) / sin(s->theta);
  
  printf("DEBUG g00r gr00 grtt grff : %f  %f  %f  %f\n", g00r, gr00, grtt, grff);
  printf("DEBUG gtrt gtff gfff : %f  %f  %f\n", gtrt, gtff, gfff);

#ifdef DEBUG                     
  float ar = 0.0;
  float aphy = 0.0;
  float atheta = 0.0;
#else

  float ar = grrr * s->vr * s->vr + grtt * s->vtheta * s->vtheta +
             grff * s->vphy * s->vphy + 2.0;
  float atheta = 2.0 * gtrt * s->vr * s->vtheta +
                 gtff * s->vphy * s->vphy;
  float aphy = 2.0 * gfrf * s->vr * s->vphy +
               gfff * s->vphy * s->vphy;
#endif


  
  s->vr -= ar * STEP;
  s->vphy -= aphy * STEP;
  s->vtheta -= atheta * STEP;
  
  s->r += s->vr * STEP;
  s->phy += s->vphy * STEP;
  s->theta += s->vtheta * STEP;
  
  s->x = -s->r * cos(s->theta) * cos(s->phy);
  s->y = -s->r * cos(s->theta) * sin(s->phy);
  s->z = -s->r * sin(s->theta);
}


void step_cartesian(particle *s)
{
  s->x -= CENTERX;
  s->y -= CENTERY;
  s->z -= CENTERZ;
  
  /*s->r = sqrt(s->x * s->x + s->y * s->y + s->z * s->z);

  float pre_giii = 0.5 * rg / s->r / s->r / s->r /
                   (1 + 0.25 * rg / s->r);

#ifdef DEBUG
  printf("DEBUG pre_giii : %f\n", pre_giii);
#endif

  float gx = pre_giii * s->x;
  float gy = pre_giii * s->y;
  float gz = pre_giii * s->z;

#define gxxx -gx
#define gyyy -gy
#define gzzz -gz

#define gxyy  gx
#define gxzz  gx
#define gyxy -gx
#define gzxz -gx
#define gyyx -gx
#define gzzx -gx

#define gyxx  gy
#define gyzz  gy
#define gxyx -gy
#define gzyz -gy
#define gxxy -gy
#define gzzy -gy

#define gzxx  gz
#define gzyy  gz
#define gxzx -gz
#define gyzy -gz
#define gxxz -gz
#define gyyz -gz

float pre_gtii = 0.5 * pow(1 + rg * 0.25 / s->r, -3.0) *
      pow(1 - rg * 0.25 / s->r, -3.0) * rg / s->r / s->r;

#ifdef DEBUG
printf("DEBUG pre_gtii : %f\n", pre_gtii);
#endif
      
float gxtt = pre_gtii * s->x;
float gytt = pre_gtii * s->y;
float gztt = pre_gtii * s->z;

#define gtxt -gxtt
#define gttx -gxtt

#define gtyt -gytt
#define gtty -gytt

#define gtzt -gztt
#define gttz -gztt


#ifdef DEBUG                     
  float ar = 0.0;
  float aphy = 0.0;
  float atheta = 0.0;
#else

  float ax = gxtt * s->vt * s->vt +
             gxxx * s->vx * s->vx +
             gxxy * s->vx * s->vy +
             gxxz * s->vx * s->vz +
             gxyy * s->vy * s->vy +
             gxzz * s->vz * s->vz +
             gxyx * s->vy * s->vx +
             gxzx * s->vz * s->vx;
             
  float ay = gytt * s->vt * s->vt +
             gyyy * s->vy * s->vy +
             gyyx * s->vy * s->vx +
             gyyz * s->vy * s->vz +
             gyxx * s->vx * s->vx +
             gyzz * s->vz * s->vz +
             gyxy * s->vx * s->vy +
             gyzy * s->vz * s->vy;
   
  float az = gztt * s->vt * s->vt +
             gzzz * s->vz * s->vz +
             gzzx * s->vz * s->vx +
             gzzy * s->vz * s->vy +
             gzxx * s->vx * s->vx +
             gzyy * s->vy * s->vy +
             gzxz * s->vx * s->vz +
             gzyz * s->vy * s->vz;
  
  float at = gtxt * s->vx * s->vt +
             gtyt * s->vy * s->vt +
             gtzt * s->vz * s->vt +
             gttx * s->vt * s->vx +
             gtty * s->vt * s->vy +
             gttz * s->vt * s->vz;
#endif


  
  s->vx -= ax * STEP;
  s->vy -= ay * STEP;
  s->vz -= az * STEP;
  s->vt -= at * STEP;*/
  
  s->x += s->vx * STEP;
  s->y += s->vy * STEP;
  s->z += s->vz * STEP;
  
  s->x += CENTERX;
  s->y += CENTERY;
  s->z += CENTERZ;
}

