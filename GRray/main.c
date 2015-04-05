
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "particle.h"


char* bitmap;
float CENTERX, CENTERY;


void init()
{
  bitmap = (char*) malloc(HEIGHT * WIDTH * 4 * sizeof(char));
  readBmp();
  
  CENTERX = 1.2;
  CENTERY = 1.2;
}


void destroy()
{
  free(bitmap);
  destroyBmp();
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
  usleep(300);
}


void idle(void)
{
  //CENTERX += 0.1;
  //CENTERY += 0.1;
  //generate();
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

