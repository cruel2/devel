
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "fluid.h"


// function declaration(s)
int which_cell(real x, real y);
void ComputeForces();
void ComputeForces_old();


// center of mass, ...
Camera camera;

// this contains the model data
Model model;


int init_model()
{
  int i;  // for iterations
  
  camera.x = 0.0f;
  camera.y = 0.0f;

  // initializes cells
  for (i = 0; i < CELLS2; i++)
  {
    model.cell[i].no_actual = 0;
    model.cell[i].no_next   = 0;
    model.cell[i].actual = (Particle**) malloc(sizeof(Particle*) * PARTICLES);
    model.cell[i].next   = (Particle**) malloc(sizeof(Particle*) * PARTICLES);
    //model.cell[i].actual = &(model.cell[i].p1);
    //model.cell[i].next   = &(model.cell[i].p2);
  }

  // initializes random-generator
  srand(time(NULL));

  // adds particles to the model
  for (i = 0; i < PARTICLES; i++)
  {
    Particle* p = (Particle*) malloc(sizeof(Particle));
    
    p->x = 1.0f * rand() / RAND_MAX - 0.5f;
    p->y = 1.0f * rand() / RAND_MAX - 0.5f - CELLX * 3.0f;
    
    p->vx = (1.0f * rand() / RAND_MAX - 0.5f) * SPEED;
    p->vy = (1.0f * rand() / RAND_MAX - 0.5f) * SPEED;
    
    p->ax = 0.0f;
    p->ay = 0.0f;

    int cn = which_cell(p->x, p->y);
    model.cell[cn].actual[model.cell[cn].no_actual] = p;
    model.cell[cn].no_actual++; // TODO post increment in previous statement
    //printf("cell: %d\n", cn);
  }

  // all OK
  return 1;
}


int step_model()
{
  int i, j; // for iteration
  static int counter = 0;
  
  counter++;
  if (counter % 100 == 0)
    printf("counter: %d\n", counter);

  for (i = 0; i < CELLS2; i++)
    model.cell[i].no_next = 0; 

  ComputeForces_old();
  //ComputeForces();

  for (i = 0; i < CELLS2; i++)
  {
    for (j = 0; j < model.cell[i].no_actual; j++)
    {
      // moves the particle
      Particle *p = model.cell[i].actual[j];
      
      p->vx += p->ax * DT;
      p->vy += p->ay * DT;
      p->x += p->vx * DT;
      p->y += p->vy * DT;
      //printf("pos x,  y : %f %f\n", p->x, p->y);
      //printf("pos vx, vy: %f %f\n\n", p->vx, p->vy);

      // adds the particle into the new array
      int cn = which_cell(p->x, p->y);
      //printf("cn: %d\n", cn);
      model.cell[cn].next[model.cell[cn].no_next] = p;
      model.cell[cn].no_next++;
    }
  }

  // swaps the arrays
  for (i = 0; i < CELLS2; i++)
  {
    // swaps actual's and next's value
    Particle **p         = model.cell[i].actual;
    model.cell[i].actual = model.cell[i].next;
    model.cell[i].next   = p;
    
    // particle numbers
    int n = model.cell[i].no_actual;
    model.cell[i].no_actual = model.cell[i].no_next;
    model.cell[i].no_next = model.cell[i].no_actual;
  }

  // all OK
  return 1;
}


int inline abs(int x)
{
  return x > 0 ? x : -x;
}


// tests the first neighbor relation
bool cell_neighbour(int base, int other)
{
  return (abs(base % CELLS - other % CELLS) < 2) &&
         (abs(base / CELLS - other / CELLS) < 2);
}


// returns the containing cell's index
int which_cell(real x, real y)
{
  // computes the cell's x index of an infinite grid
  int i = (int) floor(x / CELLX) + CELLS / 2;
  
  // the leftmost and the rightmost cells are infinite
  i = (i > 0) ?     i : 0;
  i = (i < CELLS) ? i : CELLS - 1;
  
  // debug
  //printf("x / CELLX : %f\n", x / CELLX);
  //printf("(int) floor(x / CELLX) + CELLS / 2 : %d\n", (int) floor(x / CELLX) + CELLS / 2);
  
  // computes the cell's y index of an infinite grid
  int j = (int) floor(y / CELLY) + CELLS / 2;
  
  // the topmost and the bottommost cells are infinite
  j = (j > 0) ? j : 0;
  j = (j < CELLS) ? j : CELLS - 1;
  
  // debug
  //printf("y / CELLY : %f\n", y / CELLY);
  //printf("(int) floor(y / CELLY) + CELLS / 2 : %d\n", (int) floor(y / CELLY) + CELLS / 2);
  
  // debug
  //printf("CELLS * j + i: %d\n", CELLS * j + i);
  
  return CELLS * j + i;
}


//#define PRESS_LIM 3500
real pressure(int diff)
{
  //if (diff < PRESS_LIM)  
    return PRESSURE * diff;
  //else
    //return PRESSURE * (PRESS_LIM + (diff - PRESS_LIM) * (diff - PRESS_LIM));
}


// computes the forces in primitive way
// tried to minimize the number of indirections
void ComputeForces_old()
{
  int i, j;  // for iterations
  
  // resets the acceleration values
  for (i = 0; i < CELLS2; i++)
  {
    real cx = (i % CELLS - CELLS / 2) * CELLX + CELLX / 2.0f;
    real cy = (i / CELLS - CELLS / 2) * CELLX + CELLX / 2.0f;
    
    for (j = 0; j < model.cell[i].no_actual; j++)
    {
      Particle* p = model.cell[i].actual[j];
      
      // gravity
      p->ax = 0.0f;
      p->ay = -2.0f;
      
      // position in local coordinates, origin is the cell's center
      real locx = p->x - cx;
      real locy = p->y - cy;
      
      int no_actual_ = model.cell[i].no_actual;
      
      // upper quarter
      if (locy > fabs(locx))
      {
        // if there is a cell over this  
        if (i + CELLS < CELLS2)
        {
          p->ay += pressure(no_actual_ - model.cell[i + CELLS].no_actual);
        }
      }
      
      // bottom quarter
      else if (locy < -fabs(locx))
      {
        // if there is a cell under this
        if (i - CELLS >= 0)
        {
          p->ay -= pressure(no_actual_ - model.cell[i - CELLS].no_actual);
        }
      }
      
      // right quarter
      else if (locx > fabs(locy))
      {
        if (i % CELLS < CELLS - 1)
        {
          p->ax += pressure(model.cell[i + 1].no_actual - no_actual_);
        }
      }
      
      // left quarter
      else if (locx < -fabs(locy))
      {
        if (i % CELLS > 0)
        {
          p->ax -= pressure(model.cell[i - 1].no_actual - no_actual_);
        }
      }
      
      if (p->x > CELLS / 2 * CELLX)
      {
        p->x  -= 2.0 * fabs(CELLS / 2 * CELLX - p->x);
        p->vx *= -1.0f;
      }
      
      if (p->x < -CELLS / 2 * CELLX)
      {
        p->x  += 2.0 * fabs(CELLS / 2 * CELLX + p->x);
        p->vx *= -1.0f;
      }
      
      // ceiling
      /*if (p->y > CELLS / 2 * CELLX)
      {
        p->y  -= 2.0 * fabs(CELLS / 2 * CELLX - p->y);
        p->vy *= -1.0f;
      }*/
      
      if (p->y < -CELLS / 2 * CELLX)
      {
        p->y  += 2.0 * fabs(CELLS / 2 * CELLX + p->y);
        p->vy *= -1.0f;
      }
    }
  }
}


// computes the center of mass for every cell and sum of charge
// TODO : dipolar vector
void ComputeForces()
{
  int i, j, k, l;      // for iterations
  
  for (i = 0; i < CELLS2; i++)
  {
    real q  = 0.0f;      // charge sum
    real mx = 0.0f;      // center of mass
    real my = 0.0f;      //
    
    for (j = 0; j < model.cell[i].no_actual; j++)
    {
      q += 1.0f;
      Particle* p = model.cell[i].actual[j];
      mx += p->x;
      my += p->y;
      
      //p->ax = 0.0f;
      //p->ay = 0.0f;
      
      // adding a small central force (central black hole)
      // this modification has code only here
      real r = sqrt(p->x * p->x + p->y * p->y);
      p->ax = -p->x / r / r / r * CBH_MASS;
      p->ay = -p->y / r / r / r * CBH_MASS;
    }
    
    model.cell[i].q  = q;
    if (q != 0.0f)
    {
      model.cell[i].mx = mx / q;
      model.cell[i].my = my / q;
    }
  }
  
  for (i = 0; i < CELLS2; i++)
  {
    for (k = 0; k < CELLS2; k++)
    {
      if (cell_neighbour(i, k))
      {
        for (j = 0; j < model.cell[i].no_actual; j++)
        {
          Particle* p1 = model.cell[i].actual[j];
          
          for (l = 0; l < model.cell[k].no_actual; l++)
          {
            Particle* p2 = model.cell[k].actual[l];

            // avoid division by zero
            if (p1 == p2)
              continue;
              
            real dx = (p2->x - p1->x);
            real dy = (p2->y - p1->y);

            real f3_2 = pow(dx * dx + dy * dy, 1.5f);

            p1->ax += dx / f3_2 * COULOMB;
            p1->ay += dy / f3_2 * COULOMB;
            
            //p2->ax = -p1->ax;
            //p2->ay = -p1->ay;
          }
        }
      }
      else
      {
        for (j = 0; j < model.cell[i].no_actual; j++)
        {
          Particle* p1 = model.cell[i].actual[j];
          
          if (model.cell[k].q == 0.0f)
            continue;
          
          real dx = model.cell[k].mx - p1->x;
          real dy = model.cell[k].my - p1->y;
          
          real f3_2 = pow(dx * dx + dy * dy, 1.5f);
          
          p1->ax += dx * model.cell[k].q / f3_2 * COULOMB;
          p1->ay += dy * model.cell[k].q / f3_2 * COULOMB;
        }
      }
    }
  }
}

