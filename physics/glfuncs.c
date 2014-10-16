
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdlib.h>

#include "glfuncs.h"


void keyPressed(unsigned char key, int i1, int i2)
{
  switch (key)
  {
    case 27:
    case 'q':
      destroy();
      exit(0);
      break;
  }
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDrawPixels(bmpWidth, bmpHeight, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
  glutSwapBuffers();
  usleep(1);
}


void idle(void)
{
  step();
  drawBmp();
  display();
}


void initGL(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutCreateWindow("collision model");
  glMatrixMode(GL_PROJECTION);
  glDisable(GL_DEPTH_TEST);
  glOrtho( 0, 500, 500, 0, -1, 1 );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyPressed);
  glutIdleFunc(idle); 
  glutMainLoop();
}

