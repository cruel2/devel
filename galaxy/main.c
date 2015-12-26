
/*
    main.c
    
    Main c file of galaxy
*/

#include <signal.h>  // to catch Ctrl+c
#include <stdio.h>
#include <stdlib.h>
#include "galaxy.h"


void int_handler(int dummy)
{
  printf("Viszlat!\n\n");
  exit(0);
}


int main(int argc, char** argv)
{
  int i; // for iteration
  
  signal(SIGINT, int_handler);

  init_model();
  
  for (i = 0; i < 20; i++)
  {
    //analize_model();
    step_model();
  }
  
  glut_init(argc, argv);
  


  return 1;
}
