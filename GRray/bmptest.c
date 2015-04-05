

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "particle.h"


char* bitmap;


void init()
{
  bitmap = (char*) malloc(HEIGHT * WIDTH * 4 * sizeof(char));
  readBmp();
}


void destroy()
{
  free(bitmap);
  destroyBmp();
}


int getColor(double x, double y, double z, char* color)
{
  if (z > 1.0)
  {
    int xint = (int) (x * 4.0 + 1000);
    int yint = (int) (y * 4.0 + 1000);
    
    if ((xint % 2 == 1 && yint % 2 == 0) || (xint % 2 == 0 && yint % 2 == 1))
      *color = 200;
    else
      *color = 0;
    return 1;
  }
  
  return 0;
}


void generate()
{
  int i, j, k;
  
  for (i = 0; i < HEIGHT; i++)
  {
          
    for (j = 0; j < WIDTH; j++)
    {
      bitmap[4 * (i + j * WIDTH)] =
            bmpFile[i * 3 + j * BMPROW + 2 + BMPOFFSET];
      bitmap[4 * (i + j * WIDTH) + 1] = 
            bmpFile[i * 3 + j * BMPROW + 1 + BMPOFFSET];
      bitmap[4 * (i + j * WIDTH) + 2] = 
            bmpFile[i * 3 + j * BMPROW + 0 + BMPOFFSET];
    }
  }
}


// draws image to screen
void draw()
{
  glDrawPixels(WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw();
  glutSwapBuffers();
  usleep(1);
}


void idle(void)
{
  //display();
}


void keyPressed(unsigned char key, int i1, int i2)
{
  switch (key) {
    case 27:
    case 'q':
      destroy();
      exit(0);
      break;
  }
}


int main(int argc, char **argv)
{   
    init();
      
    generate();
   
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("GR ray");
    glMatrixMode(GL_PROJECTION);
    glDisable(GL_DEPTH_TEST);
    glOrtho( 0, 500, 500, 0, -1, 1 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    glutIdleFunc(idle); 
    glutMainLoop();
    
    return 1;
}

