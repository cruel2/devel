
#include <GL/glut.h>
#include <math.h>


// rotation matrix
float mx[3][3];

// vertices
float v[8][3] = 
{
  { 0.5,  0.5,  0.5},
  { 0.5,  0.5, -0.5},
  { 0.5, -0.5, -0.5},
  { 0.5, -0.5,  0.5},
  {-0.5,  0.5,  0.5},
  {-0.5,  0.5, -0.5},
  {-0.5, -0.5, -0.5},
  {-0.5, -0.5,  0.5}
};

// edges
int e[12][2] =
{
  {0, 1},
  {1, 2},
  {2, 3},
  {3, 0},
  {0, 4},
  {1, 5},
  {2, 6},
  {3, 7},
  {4, 5},
  {5, 6},
  {6, 7},
  {7, 4}
};

void
drawBox(void)
{
  int i, j;
  
  for (i = 0; i < 8; i++)
  {
    float tx = mx[0][0] * v[i][0] + mx[0][1] * v[i][1] + mx[0][2] * v[i][2];
    float ty = mx[1][0] * v[i][0] + mx[1][1] * v[i][1] + mx[1][2] * v[i][2];
    float tz = mx[2][0] * v[i][0] + mx[2][1] * v[i][1] + mx[2][2] * v[i][2];
    
    v[i][0] = tx;
    v[i][1] = ty;
    v[i][2] = tz;
  }

  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0f, 1.0f);
  for (i = 0; i < 12; i++)
  {
    glVertex3f(v[e[i][0]][0], v[e[i][0]][1], v[e[i][0]][2]);
    glVertex3f(v[e[i][1]][0], v[e[i][1]][1], v[e[i][1]][2]);
  }
  glEnd();
}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
  glutSwapBuffers();
}

void
init(void)
{
  float fi = 0.01;
  mx[0][0] = cos(fi);
  mx[0][1] = -sin(fi);
  mx[0][2] = 0.0;
  mx[1][0] = sin(fi);
  mx[1][1] = cos(fi);
  mx[1][2] = 0.0;
  mx[2][0] = 0.0;
  mx[2][1] = 0.0;
  mx[2][2] = 1.0;  

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 40.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */
  glTranslatef(0.0, 0.0, -1.0);
  glRotatef(60, 1.0, 0.0, 0.0);
  glRotatef(-20, 0.0, 0.0, 1.0);
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("red 3D lighted cube");
  glutIdleFunc(display);
  init();
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
