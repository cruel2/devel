
/*
A megfigyelo valoszinuleg (0, 0, 5)-ben van.
A kep sikja parhuzamos az x-y sikkal es PICZ-ben van.

*/


#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define bool int
#define true 1
#define false 0


#define MODEL_MINZ -1.0
#define MODEL_MAXZ  1.0
#define NO_STEPS    100
#define INFTY       1E20   // INFINITY is reserved by math.h
#define PICZ        4.0
#define VIEWZ       5.0
#define PIXELSIZE   0.0008
#define HEIGHT      400
#define WIDTH       400


char* bitmap;

// fractal translation coordinates
float frac_x, frac_y, frac_z;


// TODO : inline legyen?
bool isOK(float cx, float cy, float cz)
{
  float x = cx * 1.2;
  float y = cy * 1.2;
  float z = cz * 1.2;
  int i;
  
  for (i = 0; i < 80; i++)
  {
    if (x * x + y * y + z * z > 4.0)
      return false;
    
    x = x*x - y*y - z*z + cx;
    y = 2*x*z + cy;
    z = 2*x*y + cz;
  }
  return true;
}


// TODO : inline legyen?
bool isOK2(float x, float y, float z)
{
  if (x*x + y*y + z*z <= 0.16 ||
     (x+0.3)*(x+0.3) + (y+0.3)*(y+0.3) + z*z < 0.16)
    return true;
  
  return false;
}


bool isOK3(float cx, float cy, float cz)
{
  float x = cx * 4.6;
  float y = cy * 4.6;
  float z = cz * 4.6;
  int i;
  
  for (i = 0; i < 80; i++)
  {
    if (x * x + y * y + z * z > 4.0)
      return false;
    
    x = ((3.0*z*z - x*x - y*y) * x * (x*x - 3.0*y*y)) / (x*x + y*y) + cx;
    y = ((3.0*z*z - x*x - y*y) * y * (3.0*x*x - y*y)) / (x*x + y*y) + cy;
    z = z*(z*z - 3.0*x*x - 3.0*y*y) + cz;
  }
  return true;
}


float getDistance(float xmul, float ymul)
{
  float x = (VIEWZ - MODEL_MAXZ) * xmul;
  float y = (VIEWZ - MODEL_MAXZ) * ymul;
  float z = MODEL_MAXZ;
  float step = (MODEL_MAXZ - MODEL_MINZ) / NO_STEPS;
  float dist;
  int i;
  
  for (i = 0; i < NO_STEPS; i++)
  {
    if (isOK(x, y, z))
    {
      //return z;
      dist = z + step;
      break;
    }
      
    x += step * xmul;
    y += step * ymul;
    z -= step;
  }
  
  //printf("z at first: %f\n", z);
  
  z = dist;
  step = step / NO_STEPS;
  x = (VIEWZ - dist) * xmul;
  y = (VIEWZ - dist) * ymul;
  
  for (i = 0; i < NO_STEPS + 2; i++)
  {
    if (isOK(x, y, z))
    {
      return sqrt(x*x + y*y + (VIEWZ - z) * (VIEWZ - z));
    }
    
    x += step * xmul;
    y += step * ymul;
    z -= step;
    //printf("second %f\n", z);
  }
  
  return INFTY;
}


void generate()
{
  int i, j;
  
  printf("\nRendering image...  0%%");
  
  for (i = 0; i < WIDTH; i++)
  {
    for (j = 0; j < HEIGHT; j++)
    {
      int idx = 4 * (j * WIDTH + i);
      
      float dist1 = getDistance((i - WIDTH/2.0 + 0.3) * PIXELSIZE,
                               (j - HEIGHT/2.0 + 0.3) * PIXELSIZE);
      if (dist1 > 10000.0)
      {
        bitmap[idx    ] = 0;
        bitmap[idx + 1] = 0;
        bitmap[idx + 2] = 0;
        bitmap[idx + 3] = 0;
        continue;
      }
      
      float dist2 = getDistance((i - WIDTH/2.0 + 0.3) * PIXELSIZE,
                               (j - HEIGHT/2.0 - 0.3) * PIXELSIZE);
      float dist3 = getDistance((i - WIDTH/2.0 - 0.3) * PIXELSIZE,
                               (j - HEIGHT/2.0 + 0.3) * PIXELSIZE);
                          
      float dx = dist3 - dist1;
      float dy = dist2 - dist1;     
      float nx = PIXELSIZE * 0.6 * dy;
      float ny = PIXELSIZE * 0.6 * dx;
      float nz = PIXELSIZE * PIXELSIZE * 0.36;
      float norm = sqrt(nx*nx + ny*ny + nz*nz);
      nx /= norm;
      ny /= norm;
      nz /= norm;
      
      float nlx = 0.6;
      float nly = 0.4;
      float nlz = sqrt(1.0 - nlx*nlx - nly*nly);
      float color = nx * nlx + ny * nly + nz * nlz;
      
      if (color >= 0.0)
        color = color * 0.7 + 0.3;
      else
        color = 0.3;
      
      /*if ((unsigned char)(nz*255) < 20)
      {
        printf("%f,   %d\n", nz, (unsigned char)(nz*255));
        printf("dx : %f       dy : %f\n", dx, dy);
        printf("nx : %f       ny : %f        nz : %f\n", nx, ny, nz);
      }*/
      
      bitmap[idx    ] = (unsigned char)(color * 255);
      bitmap[idx + 1] = (unsigned char)(color * 255);
      bitmap[idx + 2] = (unsigned char)(color * 255);
      bitmap[idx + 3] = 0;
    }
    
    printf("\b\b\b\b%3d%%", (int)(100.0f * i / WIDTH));
  }
  
  printf("\n");
}


// az x vizszintes, az y fuggoleges
// a z tengely a monitorbol kifele mutat
// a nezo z koordinataja: ~3.0
void draw(void)
{
  glDrawPixels(HEIGHT, WIDTH, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw();
  glutSwapBuffers();
}


void keyPressed(unsigned char key, int i1, int i2)
{
  switch (key) {
    case 27:
    case 'q':
      exit(0);
      break;
  }
}


void init(void)
{  
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
}


int main(int argc, char **argv)
{
  frac_x = frac_y = frac_z = 0.0f;
  
  bitmap = (char*) malloc(HEIGHT * WIDTH * 4 * sizeof(char));
  
  generate();
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("ray");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyPressed);
  init();
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}

