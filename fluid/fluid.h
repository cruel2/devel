
/*
    galaxy.h
    
    Definitions and declarations
*/

#ifndef GALAXY_H
#define GALAXY_H


#define CBH_MASS  1.0E6f       // mass of central black hole
#define CELLS     8            // number of cells in a row/column
#define CELLS2    CELLS*CELLS  // total number of cells
#define CELLX     1.0f         //
#define CELLY     1.0f         // size of cell
#define COULOMB   7.9f         // strength of the Coulomb interaction
#define DT        0.03f         // time step
#define EPS       0.001f       // epsilon value
#define MASS      1.0f         // mass of particle 
#define PARTICLES 48000        // number of particles
#define PRESSURE  -0.0001        // pressure coefficient
#define SIZE      4.0E1        // radial maximum of initial object (initial size of galaxy)
#define SPEED     0.2          // initial velocity parameter
#define ZOOM      57.5f

#define PICX      500        // picture size in pixels

#define bool      char
#define true      1
#define false     0

// test the difference between float and double: precision, performance
#define real      double


// data of a single particle's state
typedef struct Particle_struct
{
  real x,  y;
  real vx, vy;
  real ax, ay;
} Particle;


typedef struct Cell_struct
{
  // array of particles' actual state
  Particle **actual;
  int no_actual;

  // new states will be collected in this
  Particle **next;
  int no_next;
  
  //multipole coefficients
  real q;       // charge
  real px, py;  // dipole vector
  
  // center of mass
  real mx, my;
} Cell;


struct Model_struct
{
  Cell cell[CELLS2];
};


typedef struct Model_struct Model;


typedef struct Camera_struct
{
  real x, y;    // position of camera, center of mass
} Camera;


// variable declarations
extern Camera camera;
extern Model model;
extern char* bitmap;


// function declarations
int init_model();
int step_model();


#endif

