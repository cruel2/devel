#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define SQUARE_DEPTH 0.33


struct point_type
{
  float x, y, z;
};

typedef struct point_type point;

struct rectangle_type
{
  point a, b, c;
};

typedef struct rectangle_type rectangle;

struct square_type
{
  point a, b, c;
  rectangle* re; // 4 pieces
  struct square_type* sq;    // 9 pieces
  int leaf;
};

typedef struct square_type square;

square fractal[6];


void initFractal()
{
  int i;
  
  for (i = 0; i < 6; i++)
  {
    fractal[i].leaf = 1;
  }
    
  fractal[0].a.x = 0;
  fractal[0].a.y = 0;
  fractal[0].a.z = 0;
  
  fractal[1].a.x = 0;
  fractal[1].a.y = 0;
  fractal[1].a.z = 0;
  
  fractal[2].a.x = 0;
  fractal[2].a.y = 0;
  fractal[2].a.z = 0;
  
  fractal[3].a.x = 1;
  fractal[3].a.y = 1;
  fractal[3].a.z = 1;
  
  fractal[4].a.x = 1;
  fractal[4].a.y = 1;
  fractal[4].a.z = 1;
  
  fractal[5].a.x = 1;
  fractal[5].a.y = 1;
  fractal[5].a.z = 1;
  
  fractal[0].b.x = 1;
  fractal[0].b.y = 0;
  fractal[0].b.z = 0;
  
  fractal[1].b.x = 0;
  fractal[1].b.y = 0;
  fractal[1].b.z = 1;
  
  fractal[2].b.x = 0;
  fractal[2].b.y = 0;
  fractal[2].b.z = 1;
  
  fractal[3].b.x = 0;
  fractal[3].b.y = 1;
  fractal[3].b.z = 1;
  
  fractal[4].b.x = 1;
  fractal[4].b.y = 0;
  fractal[4].b.z = 1;
  
  fractal[5].b.x = 1;
  fractal[5].b.y = 1;
  fractal[5].b.z = 0;
  
  fractal[0].c.x = 0;
  fractal[0].c.y = 1;
  fractal[0].c.z = 0;
  
  fractal[1].c.x = 0;
  fractal[1].c.y = 1;
  fractal[1].c.z = 0;
  
  fractal[2].c.x = 1;
  fractal[2].c.y = 0;
  fractal[2].c.z = 0;
  
  fractal[3].c.x = 1;
  fractal[3].c.y = 0;
  fractal[3].c.z = 1;
  
  fractal[4].c.x = 1;
  fractal[4].c.y = 1;
  fractal[4].c.z = 0;
  
  fractal[5].c.x = 0;
  fractal[5].c.y = 1;
  fractal[5].c.z = 1;
}


// pointer to the array of vertices
float *vert;
int vertIdx;
int vertSize;


void printShapeData(int n)
{
  int idx = n * 15;
  
  printf("\nShape #%d\n", n);
  printf("%f   %f   %f\n", vert[idx   ], vert[idx+1 ], vert[idx+2 ]);
  printf("%f   %f   %f\n", vert[idx+3 ], vert[idx+4 ], vert[idx+5 ]);
  printf("%f   %f   %f\n", vert[idx+6 ], vert[idx+7 ], vert[idx+8 ]);
  printf("%f   %f   %f\n", vert[idx+9 ], vert[idx+10], vert[idx+11]);
  printf("%f   %f   %f\n", vert[idx+12], vert[idx+13], vert[idx+14]);
}

// computes the size of vertex array
int getVertSize(int n)
{
  int i;
  int res = 1;
  
  for (i = 1; i < n; i++)
  {
    res = res * 9 + 4;
    //res = res * 9;
  }
  
  printf("vertsize: %d\n", res * 6 * 15); // TODO ez kamu !
  return res * 6 * 15;                    // ez is !
}


// computes vertices and puts them into the vertex array
float getVertices(square* s)
{
  int i, j;
  int idx;
  float norm;
  
  if (s->leaf)
  {
    float tx, ty, tz;
    
    float v1x = s->b.x - s->a.x;
    float v1y = s->b.y - s->a.y;
    float v1z = s->b.z - s->a.z;
    
    float v2x = s->c.x - s->a.x;
    float v2y = s->c.y - s->a.y;
    float v2z = s->c.z - s->a.z;
    
    // computing normal vector
    tx = v1y * v2z - v1z * v2y;
    ty = v1z * v2x - v1x * v2z;
    tz = v1x * v2y - v1y * v2x;
    
    norm = sqrt(tx * tx + ty * ty + tz * tz);
    
    // putting normal vector into array
    vert[vertIdx++] = tx / norm;
    vert[vertIdx++] = ty / norm;
    vert[vertIdx++] = tz / norm;
    
    // a
    vert[vertIdx++] = s->a.x;
    vert[vertIdx++] = s->a.y;
    vert[vertIdx++] = s->a.z;
    
    // b
    vert[vertIdx++] = s->b.x;
    vert[vertIdx++] = s->b.y;
    vert[vertIdx++] = s->b.z;
    
    // b + c - a
    vert[vertIdx++] = s->b.x + s->c.x - s->a.x;
    vert[vertIdx++] = s->b.y + s->c.y - s->a.y;
    vert[vertIdx++] = s->b.z + s->c.z - s->a.z;
    
    // c
    vert[vertIdx++] = s->c.x;
    vert[vertIdx++] = s->c.y;
    vert[vertIdx++] = s->c.z;
  }
  else
  {
    for (i = 0; i < 9; i++)
      getVertices(&(s->sq[i]));
  }
}


void fillSquare(square* sq)
{
  int i, j;
  float v1x, v1y, v1z;
  float v2x, v2y, v2z;
  float inx, iny, inz;
  
  sq->leaf = 0;
  
  v1x = sq->b.x - sq->a.x;
  v1y = sq->b.y - sq->a.y;
  v1z = sq->b.z - sq->a.z;
  
  v2x = sq->c.x - sq->a.x;
  v2y = sq->c.y - sq->a.y;
  v2z = sq->c.z - sq->a.z;
  
  sq->sq = (square*) calloc(sizeof(square), 9);
  sq->re = (rectangle*) calloc(sizeof(rectangle), 4);
  
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
    {
      sq->sq[i*3 + j].leaf = 1;
      
      sq->sq[i*3 + j].a.x = sq->a.x + (i * v1x + j * v2x) / 3.0;
      sq->sq[i*3 + j].a.y = sq->a.y + (i * v1y + j * v2y) / 3.0;
      sq->sq[i*3 + j].a.z = sq->a.z + (i * v1z + j * v2z) / 3.0;
      
      sq->sq[i*3 + j].b.x = sq->a.x + ((i+1) * v1x + j * v2x) / 3.0;
      sq->sq[i*3 + j].b.y = sq->a.y + ((i+1) * v1y + j * v2y) / 3.0;
      sq->sq[i*3 + j].b.z = sq->a.z + ((i+1) * v1z + j * v2z) / 3.0;
      
      sq->sq[i*3 + j].c.x = sq->a.x + (i * v1x + (j+1) * v2x) / 3.0;
      sq->sq[i*3 + j].c.y = sq->a.y + (i * v1y + (j+1) * v2y) / 3.0;
      sq->sq[i*3 + j].c.z = sq->a.z + (i * v1z + (j+1) * v2z) / 3.0;
    }
  
  inx = (v1y * v2z - v1z * v2y) * SQUARE_DEPTH;
  iny = (v1z * v2x - v1x * v2z) * SQUARE_DEPTH;
  inz = (v1x * v2y - v1y * v2x) * SQUARE_DEPTH;
  
  sq->sq[4].a.x -= inx;
  sq->sq[4].a.y -= iny;
  sq->sq[4].a.z -= inz;
  
  sq->sq[4].b.x -= inx;
  sq->sq[4].b.y -= iny;
  sq->sq[4].b.z -= inz;
  
  sq->sq[4].c.x -= inx;
  sq->sq[4].c.y -= iny;
  sq->sq[4].c.z -= inz;
  
  // the four rectangles
  // the two in the upper left
  sq->re[0].a.x = sq->a.x + v1x + v2x;
  sq->re[0].a.y = sq->a.y + v1y + v2y;
  sq->re[0].a.z = sq->a.z + v1z + v2z;
  
  sq->re[1].a.x = sq->re[0].a.x;
  sq->re[1].a.y = sq->re[0].a.y;
  sq->re[1].a.z = sq->re[0].a.z;
  
  sq->re[0].b.x = sq->a.x + 2*v1x + v2x;
  sq->re[0].b.y = sq->a.y + 2*v1y + v2y;
  sq->re[0].b.z = sq->a.z + 2*v1z + v2z;
  
  sq->re[1].b.x = sq->a.x + v1x + 2*v2x;
  sq->re[1].b.y = sq->a.y + v1y + 2*v2y;
  sq->re[1].b.z = sq->a.z + v1z + 2*v2z;
  
  sq->re[0].c.x = sq->re[0].a.x + inx;
  sq->re[0].c.y = sq->re[0].a.y + iny;
  sq->re[0].c.z = sq->re[0].a.z + inz;
  
  sq->re[1].c.x = sq->re[1].a.x + inx;
  sq->re[1].c.y = sq->re[1].a.y + iny;
  sq->re[1].c.z = sq->re[1].a.z + inz;
  
  // the two lower right
  sq->re[2].a.x = sq->a.x + 2*v1x + 2*v2x;
  sq->re[2].a.y = sq->a.y + 2*v1y + 2*v2y;
  sq->re[2].a.z = sq->a.z + 2*v1z + 2*v2z;
  
  sq->re[3].a.x = sq->re[2].a.x;
  sq->re[3].a.y = sq->re[2].a.y;
  sq->re[3].a.z = sq->re[2].a.z;
  
  sq->re[2].b.x = sq->a.x + 2*v1x + v2x;
  sq->re[2].b.y = sq->a.y + 2*v1y + v2y;
  sq->re[2].b.z = sq->a.z + 2*v1z + v2z;
  
  sq->re[3].b.x = sq->a.x + v1x + 2*v2x;
  sq->re[3].b.y = sq->a.y + v1y + 2*v2y;
  sq->re[3].b.z = sq->a.z + v1z + 2*v2z;
  
  sq->re[2].c.x = sq->a.x + inx;
  sq->re[2].c.y = sq->a.y + iny;
  sq->re[2].c.z = sq->a.z + inz;
  
  sq->re[3].c.x = sq->a.x + inx;
  sq->re[3].c.y = sq->a.y + iny;
  sq->re[3].c.z = sq->a.z + inz;
}


GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */


void drawBox(void)
{
  int i;

  for (i = 0; i < 486; i++) {
    glBegin(GL_QUADS);
    glNormal3fv(&vert[i*15]);
    glVertex3fv(&vert[i*15 + 3]);
    glVertex3fv(&vert[i*15 + 6]);
    glVertex3fv(&vert[i*15 + 9]);
    glVertex3fv(&vert[i*15 + 12]);
    glEnd();
  }
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
  glutSwapBuffers();
}


void init(void)
{
  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

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


int main(int argc, char **argv)
{
  int i, j;
  
  initFractal();
  for (i = 0; i < 6; i++)
    fillSquare(&fractal[i]);
    
  for (i = 0; i < 6; i++)
    for (j = 0; j < 9; j++)
      fillSquare(&fractal[i].sq[j]);
  
  // allocating memory for vertex array
  vertIdx = 0;
  vertSize = getVertSize(3);
  vert = (float*) malloc(vertSize * sizeof(float));
  
  // filling the vertex array
  for (i = 0; i < 6; i++)
    getVertices(&fractal[i]);
  
  // debug  
  for (i = 0; i < 6; i++)  
    printShapeData(i);
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("red 3D lighted cube");
  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}

