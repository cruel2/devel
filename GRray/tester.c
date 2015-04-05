
#include <stdio.h>

#include "particle.h"

#undef CENTERZ
#define CENTERZ 0.0
#define DEBUG

particle s_cart;
particle s_sphe;


void init()
{
  s_cart.x =  3.5;
  s_cart.y =  3.5;
  s_cart.z = -3.0;
  
  s_sphe.x =  3.5;
  s_sphe.y =  3.5;
  s_sphe.z = -3.0;
  
  s_cart.vx =  0.0;
  s_cart.vy =  0.0;
  s_cart.vz =  C;
  s_cart.vt =  C;
  
  s_sphe.vx =  0.0;
  s_sphe.vy =  0.0;
  s_sphe.vz =  C;
  s_sphe.vt =  C;
}


// creates only one step
void test1()
{
  init();
  
  Cart2Sphe(&s_sphe);
  
  printf("cart (x, y, z): %f  %f  %f\n", s_cart.x, s_cart.y, s_cart.z);
  printf("sphe (x, y, z): %f  %f  %f\n", s_sphe.x, s_sphe.y, s_sphe.z);
  
  step_cartesian(&s_cart);
  step_spherical(&s_sphe);
 
  printf("cart (x, y, z): %f  %f  %f\n", s_cart.x, s_cart.y, s_cart.z);
  printf("sphe (x, y, z): %f  %f  %f\n", s_sphe.x, s_sphe.y, s_sphe.z);
}


// tests one path
void test2()
{
  int i;
  
  s_cart.x = 1.0;
  s_cart.y = 1.0;
  s_cart.z = -1.0;
  
  s_cart.vx = s_cart.vy = 0.0;
  s_cart.vz = C;
  s_cart.vt = C;
  
  for (i = 0; i < 100; i++)
  {
    printf("cart (x, y, z): %f  %f  %f\n", s_cart.x, s_cart.y, s_cart.z);
    printf("cart (vx, vy, vz): %f  %f  %f\n", s_cart.vx, s_cart.vy, s_cart.vz);
  
    step_cartesian(&s_cart);
  }
}


int main()
{
  test2();
  
  return 1;
}

