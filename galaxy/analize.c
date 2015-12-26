
#include <math.h>
#include <stdio.h>
#include "galaxy.h"


/*
    Computes the center-of-mass of whole model
    Puts the result in camera structure
*/
void center_of_mass()
{
  int i, j;    // for iteration
  real mx = 0.0f;
  real my = 0.0f;
  
  for (i = 0; i < CELLS2; i++)
    for (j = 0; j < model.cell[i].no_actual; j++)
    {
      Particle* p = model.cell[i].actual[j];
      
      mx  += p->x;
      my  += p->y;
    }
    
  camera.x = mx / PARTICLES;
  camera.y = my / PARTICLES;
}


/*
    Counts the free particles
    Free if radial speed is larger than escape velocity
*/
void count_free_particles()
{
  int i, j;  // for iteration
  int counter = 0;
  
  for (i = 0; i < CELLS2; i++)
  {
    for (j = 0; j < model.cell[i].no_actual; j++)
    {
      Particle* p = model.cell[i].actual[j];
      real r = sqrt(p->x * p->x + p->y * p->y);
      
      // radial velocity
      real v = (p->vx * p->x + p->vy * p->y) / r;
      
      // compares radial velocity with local escape velocity ('s first approach)
      // v = sqrt(GM/r) , wikipedia
      if (sqrt(COULOMB * MASS * PARTICLES / r) < v)
        counter++;
    }
  }
  
  printf("Escaping: %d\n", counter);
}


/*
    computes the kinetic energy sum
*/
void energy_sum()
{
  int i, j;  // for iteration
  real esum = 0.0f;
  real vx = 0.0f;
  real vy = 0.0f;
  
  for (i = 0; i < CELLS2; i++)
    for (j = 0; j < model.cell[i].no_actual; j++)
    {
      Particle* p = model.cell[i].actual[j];
      
      esum += (p->vx * p->vx + p->vy * p->vy) * MASS / 2.0f;
      vx  += p->vx;
      vy  += p->vy;
      
      if (i == 8)
        printf("i j  vx  vy: %d %d  %f  %f\n", i, j, p->vx, p->vy);
    }
    
  printf("Esum  vx  vy: %f  %f  %f\n", esum, vx, vy);
}


/*
    prints the non-empty cells' population
*/
int analize_model()
{
  int i;  // for iterations
  
  for (i = 0; i < CELLS2; i++)
  {
    if (model.cell[i].no_actual > 0)
      printf("%d. cell's population: %d\n", i, model.cell[i].no_actual);
  }
}

