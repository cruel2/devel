
#include <GL/glut.h>            /* Open GL Util    OpenGL*/
#include <stdio.h>
#include "fluid.h"
#include <time.h>


char* bitmap;
clock_t start;


bool is_inside(int x, int y)
{
  return (x >= 0 && y >= 0 && x < PICX && y < PICX);
}


// display function
void display_call()
{
  int i, j;  // for iterations
  static int counter = 0;         // for time measurement
  
  
  counter++;
  
  if (counter == 100)
  {
    clock_t end = clock();
    float seconds = (float) (end - start) / CLOCKS_PER_SEC;
    printf("time %f\n", seconds);
  }
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  glColor3f(1,1,1);
  
  step_model();
  
  for (i = 0; i < PICX * PICX * 4; i++)
    bitmap[i] = 0;
  
  // bitmap coordinates for wall edges
  int wall_x1 = (int) (-ZOOM * CELLS / 2 * CELLX) + PICX / 2;
  int wall_x2 = (int) ( ZOOM * CELLS / 2 * CELLX) + PICX / 2;
  int wall_y1 = (int) (-ZOOM * CELLS / 2 * CELLX) + PICX / 2;
  int wall_y2 = (int) ( ZOOM * CELLS / 2 * CELLX) + PICX / 2;
  
  // vertical walls
  for (i = wall_y1; i <= wall_y2; i++)
  {
    if (is_inside(wall_x1, i))
      bitmap[(wall_x1 + i * PICX) * 3 + 1] = 200;
      
    if (is_inside(wall_x2, i))
      bitmap[(wall_x2 + i * PICX) * 3 + 1] = 200;
  }
  
  // horizontal walls
  for (i = wall_x1; i <= wall_x2; i++)
  {
    if (is_inside(i, wall_y1))
      bitmap[(i + wall_y1 * PICX) * 3 + 1] = 200;
      
    //if (is_inside(i, wall_y2))
    //  bitmap[(i + wall_y2 * PICX) * 3 + 1] = 200;
  }
  
  
  for (i = 0; i < CELLS2; i++)
  {
    for (j = 0; j < model.cell[i].no_actual; j++)
    {
      //printf("i  j  no ; %d  %d  %d\n", i, j, model.cell[i].no_actual);
      Particle* p = model.cell[i].actual[j];
      int x = (int) (ZOOM * (p->x - camera.x)) + PICX / 2;
      int y = (int) (ZOOM * (p->y - camera.y)) + PICX / 2;
      if (x > 0 && y > 0 && x < PICX && y < PICX)
      {
        bitmap[(x + y * PICX) * 3] = 200;
      }
    }
  }
  
  glDrawPixels(PICX, PICX, GL_RGB, GL_UNSIGNED_BYTE, bitmap);
        
  glutSwapBuffers();
} /* end func displayCall */


// keyboard handler function, for further use
void key_pressed(unsigned char c)
{
  if (c == 'a')
  {
  }
}


// initializes openGL stuff and starts main loop
int glut_init(int argc, char *argv[]) {
  bitmap = (char*) malloc(PICX * PICX * 4);
  start = clock();
  
  // initializes graphical interface
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(PICX, PICX);
  glutInitWindowPosition(300, 200);
  glutCreateWindow("Galaxy");
  
  // for keyboard handler function
  //glutKeyboardFunc(key_pressed);
  
  // draws if resize window
  glutDisplayFunc(display_call);
  
  // draws always
  glutIdleFunc(display_call);
  
  // ???  
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 1.0 * PICX);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glutMainLoop();
  return 1;
}


