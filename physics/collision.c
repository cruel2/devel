
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "glfuncs.h"


struct particle_
{
  float oldx, oldy;
  float x,  y;
  float vx, vy;
  float fx, fy;
};

typedef struct particle_ particle;


char* bitmap;
float dt;
float powerConst;
int bmpHeight, bmpWidth;
float boxHeight, boxWidth;
int noParticles;
particle* part;


void init()
{
  int i;
  
  part = (particle*) malloc(sizeof(particle) * noParticles);
  bitmap = (char*) calloc(sizeof(char), bmpHeight * bmpWidth * 4);
  
  // dummy initialization
  part[0].x =  4.5f;
  part[0].y =  4.0f;
  part[1].x =  4.5f;
  part[1].y =  5.0f;
  
  part[0].vx =  0.3f;
  part[0].vy =  0.4f;
  part[1].vx = -0.3f;
  part[1].vy =  0.4f;
  
  /*for (i = 0; i < noParticles; i++) TODO
  {
    
  }*/
}


void step()
{
  int i, j;
  
  for (i = 0; i < noParticles; i++)
  {
    part[i].oldx = part[i].x;
    part[i].oldy = part[i].y;
    part[i].fx = 0.0f;
    part[i].fy = 0.0f;
  }
  
  for (i = 0; i < noParticles; i++)
    for (j = 0; j < i; j++)
    {
      // we need a normal vector between the particles
      float nx = part[i].x - part[j].x;
      float ny = part[i].y - part[j].y;
      
      //distance of particles
      float r = sqrt(nx * nx + ny * ny);
      
      nx /= r;
      ny /= r;
      
      part[i].fx += nx / r * powerConst;
      part[i].fy += ny / r * powerConst;
      
      part[j].fx -= nx / r * powerConst;
      part[j].fy -= ny / r * powerConst;
    }
  
  for (i = 0; i < noParticles; i++)
  {
    part[i].vx += part[i].fx * dt;
    part[i].vy += part[i].fy * dt;
    
    part[i].x += part[i].vx * dt;
    part[i].y += part[i].vy * dt;
  }
  
  for (i = 0; i < noParticles; i++)
  {
    if (part[i].x < 0.0f)
    {
      part[i].x  = -part[i].x;
      part[i].vx = -part[i].vx;
    }
    else if (part[i].x > boxWidth)
    { 
      part[i].x  = 2 * boxWidth - part[i].x;
      part[i].vx = -part[i].vx;
    }
    
    if (part[i].y < 0.0f)
    {
      part[i].y  = -part[i].y;
      part[i].vy = -part[i].vy;
    }
    else if (part[i].y > boxHeight)
    { 
      part[i].y  = 2 * boxHeight - part[i].y;
      part[i].vy = -part[i].vy;
    }
  }
  
  //printf("coord x: %f    y: %f\n", part[0].x, part[i].y);
}


void drawBmp()
{
  int i;
  
  // erasing old pixels
  for (i = 0; i < noParticles; i++)
  {
    int xc = (int) (part[i].oldx / boxWidth  * bmpWidth);
    int yc = (int) (part[i].oldy / boxHeight * bmpHeight);
    
    bitmap[(yc * bmpWidth + xc) * 4]     = 0;
    bitmap[(yc * bmpWidth + xc) * 4 + 1] = 0;
  }
  
  // drawing new pixels
  for (i = 0; i < noParticles; i++)
  {
    int xc = (int) (part[i].x / boxWidth  * bmpWidth);
    int yc = (int) (part[i].y / boxHeight * bmpHeight);
    //printf("coord x: %f    y: %f\n", part[0].x, part[i].y);
    //printf("coord x: %d    y: %d\n", xc, yc);
    
    bitmap[(yc * bmpWidth + xc) * 4]     = 255;
    bitmap[(yc * bmpWidth + xc) * 4 + 1] = 255;
  }
}


void destroy()
{
  free(part);
  free(bitmap);
}


// TODO parameter: reszecskek szama
int main(int argc, char** argv)
{
  dt = 0.1f;
  noParticles = 2;
  powerConst = 1.0f;
  bmpHeight = 450;
  bmpWidth = 450;
  boxHeight = 10.0f;
  boxWidth = 10.0f;
  
  init();
  initGL();
  
  destroy();
}

