
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>


// one of the four space coordinates has to be ignored
int projectedCoord;

// rotation matrices
// in the x-y plane
float mx[5][5];
// general
float mx2[5][5];

// vertices
float v[32][5] = 
{
  { 0.5, 0.5,  0.5,  0.5,  0.5},
  { 0.5, 0.5,  0.5,  0.5, -0.5},
  { 0.5, 0.5,  0.5, -0.5, -0.5},
  { 0.5, 0.5,  0.5, -0.5,  0.5},
  { 0.5, 0.5, -0.5,  0.5,  0.5},
  { 0.5, 0.5, -0.5,  0.5, -0.5},
  { 0.5, 0.5, -0.5, -0.5, -0.5},
  { 0.5, 0.5, -0.5, -0.5,  0.5},
  { 0.5,-0.5,  0.5,  0.5,  0.5},
  { 0.5,-0.5,  0.5,  0.5, -0.5},
  { 0.5,-0.5,  0.5, -0.5, -0.5},
  { 0.5,-0.5,  0.5, -0.5,  0.5},
  { 0.5,-0.5, -0.5,  0.5,  0.5},
  { 0.5,-0.5, -0.5,  0.5, -0.5},
  { 0.5,-0.5, -0.5, -0.5, -0.5},
  { 0.5,-0.5, -0.5, -0.5,  0.5},
  
  {-0.5, 0.5,  0.5,  0.5,  0.5},
  {-0.5, 0.5,  0.5,  0.5, -0.5},
  {-0.5, 0.5,  0.5, -0.5, -0.5},
  {-0.5, 0.5,  0.5, -0.5,  0.5},
  {-0.5, 0.5, -0.5,  0.5,  0.5},
  {-0.5, 0.5, -0.5,  0.5, -0.5},
  {-0.5, 0.5, -0.5, -0.5, -0.5},
  {-0.5, 0.5, -0.5, -0.5,  0.5},
  {-0.5,-0.5,  0.5,  0.5,  0.5},
  {-0.5,-0.5,  0.5,  0.5, -0.5},
  {-0.5,-0.5,  0.5, -0.5, -0.5},
  {-0.5,-0.5,  0.5, -0.5,  0.5},
  {-0.5,-0.5, -0.5,  0.5,  0.5},
  {-0.5,-0.5, -0.5,  0.5, -0.5},
  {-0.5,-0.5, -0.5, -0.5, -0.5},
  {-0.5,-0.5, -0.5, -0.5,  0.5},
};

// edges
int e[80][2] =
{
  // 1. horizontal face
  { 0,  1},
  { 1,  2},
  { 2,  3},
  { 3,  0},
  
  // 2. horizontal face
  { 4,  5},
  { 5,  6},
  { 6,  7},
  { 7,  4},
  
  // 3. horizontal face
  { 8,  9},
  { 9, 10},
  {10, 11},
  {11,  8},
  
  // 4. horizontal face
  {12, 13},
  {13, 14},
  {14, 15},
  {15, 12},
  
  // 5. horizontal face
  {16, 17},
  {17, 18},
  {18, 19},
  {19, 16},
  
  // 6. horizontal face
  {20, 21},
  {21, 22},
  {22, 23},
  {23, 20},
  
  // 7. horizontal face
  {24, 25},
  {25, 26},
  {26, 27},
  {27, 24},
  
  // 8. horizontal face
  {28, 29},
  {29, 30},
  {30, 31},
  {31, 28},
  
  // 1. cube's side edges
  { 0,  4},
  { 1,  5},
  { 2,  6},
  { 3,  7},
  
  // 2. cube's side edges
  { 8, 12},
  { 9, 13},
  {10, 14},
  {11, 15},
  
  // 3. cube's side edges
  {16, 20},
  {17, 21},
  {18, 22},
  {19, 23},
  
  // 4. cube's side edges
  {24, 28},
  {25, 29},
  {26, 30},
  {27, 31},
  
  // connecting the cubes 1
  { 0,  8},
  { 1,  9},
  { 2, 10},
  { 3, 11},
  { 4, 12},
  { 5, 13},
  { 6, 14},
  { 7, 15},
  
  // connecting the cubes 2
  {16, 24},
  {17, 25},
  {18, 26},
  {19, 27},
  {20, 28},
  {21, 29},
  {22, 30},
  {23, 31},
  
  // connecting the hypercubes
  { 0, 16},
  { 1, 17},
  { 2, 18},
  { 3, 19},
  { 4, 20},
  { 5, 21},
  { 6, 22},
  { 7, 23},
  { 8, 24},
  { 9, 25},
  {10, 26},
  {11, 27},
  {12, 28},
  {13, 29},
  {14, 30},
  {15, 31},
};


float delta(int i, int j)
{
  return (i==j)?1.0f:0.0f;
}


void drawBox(void)
{
  int i, j;
  
  for (i = 0; i < 16; i++)
  {
    float tx, ty, tz, tu, tv; 
    
    if (projectedCoord < 5)
    {
      tx = mx[0][0] * v[i][0] + mx[0][1] * v[i][1] +
           mx[0][2] * v[i][2] + mx[0][3] * v[i][3] +
           mx[0][4] * v[i][4];
      ty = mx[1][0] * v[i][0] + mx[1][1] * v[i][1] +
           mx[1][2] * v[i][2] + mx[1][3] * v[i][3] +
           mx[1][4] * v[i][4];
      tz = mx[2][0] * v[i][0] + mx[2][1] * v[i][1] +
           mx[2][2] * v[i][2] + mx[2][3] * v[i][3] +
           mx[2][4] * v[i][4];
      tu = mx[3][0] * v[i][0] + mx[3][1] * v[i][1] +
           mx[3][2] * v[i][2] + mx[3][3] * v[i][3] +
           mx[3][4] * v[i][4];
      tv = mx[4][0] * v[i][0] + mx[4][1] * v[i][1] +
           mx[4][2] * v[i][2] + mx[4][3] * v[i][3] +
           mx[4][4] * v[i][4];
    }
    else
    {
      tx = mx2[0][0] * v[i][0] + mx2[0][1] * v[i][1] +
           mx2[0][2] * v[i][2] + mx2[0][3] * v[i][3] +
           mx2[0][4] * v[i][4];
      ty = mx2[1][0] * v[i][0] + mx2[1][1] * v[i][1] +
           mx2[1][2] * v[i][2] + mx2[1][3] * v[i][3] +
           mx2[1][4] * v[i][4];
      tz = mx2[2][0] * v[i][0] + mx2[2][1] * v[i][1] +
           mx2[2][2] * v[i][2] + mx2[2][3] * v[i][3] +
           mx2[2][4] * v[i][4];
      tu = mx2[3][0] * v[i][0] + mx2[3][1] * v[i][1] +
           mx2[3][2] * v[i][2] + mx2[3][3] * v[i][3] +
           mx2[3][4] * v[i][4];
      tv = mx2[4][0] * v[i][0] + mx2[4][1] * v[i][1] +
           mx2[4][2] * v[i][2] + mx2[4][3] * v[i][3] +
           mx2[4][4] * v[i][4];
    }
    
    v[i][0] = tx;
    v[i][1] = ty;
    v[i][2] = tz;
    v[i][3] = tu;
    v[i][4] = tv;
  }

  glBegin(GL_LINES);
  switch (projectedCoord % 10)
  {
    case 0:
      glColor3f(0.5f, 0.0f, 0.0f);
      break;
    case 1:
      glColor3f(0.0f, 0.5f, 0.0f);
      break;
    case 2:
      glColor3f(0.5f, 0.5f, 0.0f);
      break;
    case 3:
      glColor3f(0.0f, 0.0f, 0.5f);
      break;
    case 4:
      glColor3f(0.5f, 0.0f, 0.5f);
      break;
    case 5:
      glColor3f(1.0f, 0.0f, 0.0f);
      break;
    case 6:
      glColor3f(0.0f, 1.0f, 0.0f);
      break;
    case 7:
      glColor3f(1.0f, 1.0f, 0.0f);
      break;
    case 8:
      glColor3f(0.0f, 0.0f, 1.0f);
      break;
    case 9:
      glColor3f(1.0f, 0.0f, 1.0f);
      break;
    default:
      fprintf(stderr, "Something is really wrong here...\n");
      exit(-1);
      break;
  }
    
  for (i = 0; i < 80; i++)
  {
    switch (projectedCoord % 10)
    {
      case 0:
        glVertex3f(v[e[i][0]][2], v[e[i][0]][3], v[e[i][0]][4]);
        glVertex3f(v[e[i][1]][2], v[e[i][1]][3], v[e[i][1]][4]);
        break;
      case 1:
        glVertex3f(v[e[i][0]][1], v[e[i][0]][3], v[e[i][0]][4]);
        glVertex3f(v[e[i][1]][1], v[e[i][1]][3], v[e[i][1]][4]);
        break;
      case 2:
        glVertex3f(v[e[i][0]][0], v[e[i][0]][3], v[e[i][0]][4]);
        glVertex3f(v[e[i][1]][0], v[e[i][1]][3], v[e[i][1]][4]);
        break;
      case 3:
        glVertex3f(v[e[i][0]][1], v[e[i][0]][2], v[e[i][0]][4]);
        glVertex3f(v[e[i][1]][1], v[e[i][1]][2], v[e[i][1]][4]);
        break;
      case 4:
        glVertex3f(v[e[i][0]][0], v[e[i][0]][2], v[e[i][0]][4]);
        glVertex3f(v[e[i][1]][0], v[e[i][1]][2], v[e[i][1]][4]);
        break;
      case 5:
        glVertex3f(v[e[i][0]][0], v[e[i][0]][1], v[e[i][0]][4]);
        glVertex3f(v[e[i][1]][0], v[e[i][1]][1], v[e[i][1]][4]);
        break;
      case 6:
        glVertex3f(v[e[i][0]][1], v[e[i][0]][2], v[e[i][0]][3]);
        glVertex3f(v[e[i][1]][1], v[e[i][1]][2], v[e[i][1]][3]);
        break;
      case 7:
        glVertex3f(v[e[i][0]][0], v[e[i][0]][2], v[e[i][0]][3]);
        glVertex3f(v[e[i][1]][0], v[e[i][1]][2], v[e[i][1]][3]);
        break;
      case 8:
        glVertex3f(v[e[i][0]][0], v[e[i][0]][1], v[e[i][0]][3]);
        glVertex3f(v[e[i][1]][0], v[e[i][1]][1], v[e[i][1]][3]);
        break;
      case 9:
        glVertex3f(v[e[i][0]][0], v[e[i][0]][1], v[e[i][0]][2]);
        glVertex3f(v[e[i][1]][0], v[e[i][1]][1], v[e[i][1]][2]);
        break;  
      // TODO
      default:
        fprintf(stderr, "Something is really wrong here...\n");
        exit(-1);
        break;
    }
  }
  glEnd();
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
  glutSwapBuffers();
}


void init(void)
{
  float fi = 0.01;
  float norm;
  float v1[5], v2[5];
  int i, j;
  
  // building the x-y plane's rotation matrix
  mx[0][0] = cos(fi);
  mx[0][1] = -sin(fi);
  mx[0][2] = 0.0;
  mx[0][3] = 0.0;
  mx[0][4] = 0.0;
  
  mx[1][0] = sin(fi);
  mx[1][1] = cos(fi);
  mx[1][2] = 0.0;
  mx[1][3] = 0.0;
  mx[1][4] = 0.0;
  
  mx[2][0] = 0.0;
  mx[2][1] = 0.0;
  mx[2][2] = 1.0;
  mx[2][3] = 0.0;
  mx[2][4] = 0.0;
  
  mx[3][0] = 0.0;
  mx[3][1] = 0.0;
  mx[3][2] = 0.0;
  mx[3][3] = 1.0;
  mx[3][4] = 0.0;
  
  mx[4][0] = 0.0;
  mx[4][1] = 0.0;
  mx[4][2] = 0.0;
  mx[4][3] = 0.0;
  mx[4][4] = 1.0;
  
  // building the general rotation matrix
  v1[0] =  2.0;
  v1[1] = -3.0;
  v1[2] =  4.0;
  v1[3] =  5.0;
  v1[4] = -6.0;
  
  v2[0] = -1.0;
  v2[1] = -3.0;
  v2[2] =  2.0;
  v2[3] = -2.0;
  v2[4] = - (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] + v1[3]*v2[3]) / v1[4];
  
  norm = sqrt(v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2] + v1[3]*v1[3] + v1[4]*v1[4]);
  v1[0] /= norm;
  v1[1] /= norm;
  v1[2] /= norm;
  v1[3] /= norm;
  v1[4] /= norm;
  
  norm = sqrt(v2[0]*v2[0] + v2[1]*v2[1] + v2[2]*v2[2] + v2[3]*v2[3] + v2[4]*v2[4]);
  v2[0] /= norm;
  v2[1] /= norm;
  v2[2] /= norm;
  v2[3] /= norm;
  v2[4] /= norm;
  
  for (i = 0; i < 5; i++)
    for (j = 0; j < 5; j++)
    {
      mx2[i][j] = (v1[i]*v1[j] + v2[i]*v2[j]) * (cos(fi) - 1.0f) +
                  (v1[i]*v2[j] - v2[i]*v1[j]) * sin(fi) +
                  delta(i, j);
    }
  
  projectedCoord = 0;

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


void keyPressed(unsigned char key, int i1, int i2)
{
  switch (key) {
  case 27:
  case 'q':
    exit(0);
    break;
  case ' ':
    projectedCoord = (projectedCoord + 1) % 8;
    break;
  }
}


int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("red 3D lighted cube");
  glutKeyboardFunc(keyPressed);
  glutIdleFunc(display);
  init();
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}

