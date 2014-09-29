

#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "frac.h"


#define SIZE 0.001
#define PICS 450


#define _height PICS
#define _width PICS
#define _bitsperpixel 32
#define _planes 1
#define _compression 0
//#define _pixelbytesize _height*_width*_bitsperpixel/8
//#define _filesize _pixelbytesize+sizeof(bitmap)
#define _xpixelpermeter 0x130B //2835 , 72 DPI
#define _ypixelpermeter 0x130B //2835 , 72 DPI
#define pixel 0xFF
#pragma pack(push,1)
typedef struct {
    uint8_t signature[2];
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
} fileheader_type;
typedef struct {
    uint32_t dibheadersize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t ypixelpermeter;
    uint32_t xpixelpermeter;
    uint32_t numcolorspallette;
    uint32_t mostimpcolor;
} bitmapinfoheader_type;
typedef struct {
    fileheader_type fileheader;
    bitmapinfoheader_type bitmapinfoheader;
} bitmap_type;
#pragma pack(pop)


// actual position, zoom
double zoom = 1.0;
float zoom2 = 0.00075;
float xpos = 0.0;
float ypos = 0.0;


// Values come from the config file
int WIDTH;
int HEIGHT;
int BITSPERPIXEL;
int PIXELBYTESIZE;
int FILESIZE;
bool WRITECOORDS;
int STEPSIZE1;
int STEPSIZE2;
bool HYPHENNEEDED;
bool PRINTDOUBLE;
int NOTHREADS;

// this is the number of the latest picture in the pictures' folder
int fileNumber;

int outside = 0;
char bitmap[PICS*PICS*4];
pthread_mutex_t bitmap_mutex;

// Points to the inclusion function
bool (*isOK)(int, int, int*);


inline unsigned char min(int a, int b)
{
    return (unsigned char) (a > b? b:a);
}


int readConfig()
{
  int temp = 0;
  FILE *fp = fopen("config.ini", "r");
  if (!fp)
  {
    printf("Can't open config file!\n");
    return -1;
  }
  
  if (!fscanf(fp, "WIDTH = %d\n", &WIDTH))
  {
    printf("Couldn't read width!\n");
    return -2;
  }
    
  if (!fscanf(fp, "HEIGHT = %d\n", &HEIGHT))
  {
    printf("Couldn't read height!\n");
    return -2;
  }
  
  if (!fscanf(fp, "BITSPERPIXEL = %d\n", &temp))
  {
    printf("Couldn't read bitsperpixel!\n");
    return -2;
  }
  
  if (!fscanf(fp, "WRITECOORDS = %d\n", &temp))
  {
    printf("Couldn't read write coords!\n");
    return -2;
  }
  if (temp == 1)
    WRITECOORDS = true;
  else if (temp == 0)
    WRITECOORDS = false;
  else
  {
    printf("Invalid value for write coords!\n");
    return -2;
  }
  
  if (!fscanf(fp, "STEPSIZE1 = %d\n", &STEPSIZE1))
  {
    printf("Couldn't read step size 1!\n");
    return -2;
  }

  if (!fscanf(fp, "STEPSIZE2 = %d\n", &STEPSIZE2))
  {
    printf("Couldn't read step size 2!\n");
    return -2;
  }
  
  if (!fscanf(fp, "HYPHENNEEDED = %d\n", &temp))
  {
    printf("Couldn't read hyphen needed!\n");
    return -2;
  }
  
  if (temp == 1)
    HYPHENNEEDED = true;
  else if (temp == 0)
    HYPHENNEEDED = false;
  else
  {
    printf("Invalid value for hyphen needed!\n");
    return -2;
  }
  
  if (!fscanf(fp, "PRINTDOUBLE = %d\n", &temp))
  {
    printf("Couldn't read print double!\n");
    return -2;
  }
  
  if (temp == 1)
    PRINTDOUBLE = true;
  else if (temp == 0)
    PRINTDOUBLE = false;
  else
  {
    printf("Invalid value for print double!\n");
    return -2;
  }
  
  if (!fscanf(fp, "NOTHREADS = %d\n", &NOTHREADS))
  {
    printf("Couldn't read number of threads!\n");
    return -2;
  }
  
  PIXELBYTESIZE = WIDTH * HEIGHT * _bitsperpixel / 8;
  FILESIZE = PIXELBYTESIZE + sizeof(bitmap);  // TODO ez igy biztos, hogy jo?
  
  if (0)
  {
    printf("Config file reading - DEBUG\n");
    printf("WIDTH:        %d\n", WIDTH);
    printf("HEIGHT:       %d\n", HEIGHT);
    printf("BITSPERPIXEL: %d\n", BITSPERPIXEL);
    printf("WRITECOORDS:  %d\n", (int)WRITECOORDS);
    printf("STEPSIZE1:    %d\n", STEPSIZE1);
    printf("STEPSIZE2:    %d\n", STEPSIZE2);
    printf("HYPHENNEEDED: %d\n", (int)HYPHENNEEDED);
    printf("PRINTDOUBLE:  %d\n", (int)PRINTDOUBLE);
    printf("NOTHREADS:    %d\n", NOTHREADS);
  }
  
  return 1;
}


void writeBmp()
{
  char fileName[13];
  
  fileNumber++;
  strncpy(fileName, "test", 4);
  sprintf(fileName + 4, "%4.4d", fileNumber);
  strncpy(fileName + 8, ".bmp", 4);
  
  FILE *fp = fopen(fileName,"wb");
  bitmap_type *pbitmap  = (bitmap_type*)calloc(1,sizeof(bitmap_type));
  uint8_t *pixelbuffer = (uint8_t*)(&bitmap);
  //strcpy(pbitmap->fileheader.signature,"BM");
  pbitmap->fileheader.signature[0] = 'B';
  pbitmap->fileheader.signature[1] = 'M';
  pbitmap->fileheader.filesize = FILESIZE;
  pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap_type);
  pbitmap->bitmapinfoheader.dibheadersize =sizeof(bitmapinfoheader_type);
  pbitmap->bitmapinfoheader.width = WIDTH;
  pbitmap->bitmapinfoheader.height = HEIGHT;
  pbitmap->bitmapinfoheader.planes = _planes;
  pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
  pbitmap->bitmapinfoheader.compression = _compression;
  pbitmap->bitmapinfoheader.imagesize = PIXELBYTESIZE;
  pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter ;
  pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter ;
  pbitmap->bitmapinfoheader.numcolorspallette = 0;
  fwrite (pbitmap, 1, sizeof(bitmap_type),fp);
  
  fwrite(pixelbuffer, 1, PIXELBYTESIZE, fp);
  fclose(fp);
  free(pbitmap);
}


void* generate_thread(void* arg_)
{
  int i, j, ii, jj;
  int outside;
  generateThreadArg* arg = (generateThreadArg*) arg_;
  
  for (i = arg->shift; i < PICS; i+=arg->step)
  {
    for (j = 0; j < PICS; j++)
    {
      int sum = 0;
      for (ii = 0; ii < 4; ii++)
        for (jj = 0; jj < 4; jj++)
        {
          if (isOK(i*4-60+ii, j*4+30+jj, &outside))
            sum += 1;
        }
      
      pthread_mutex_lock(&bitmap_mutex);    
      if (sum > 0)
      {
        bitmap[(PICS * j + i) * 4] = 15 * sum;
        bitmap[(PICS * j + i) * 4 + 1] = 0;
        bitmap[(PICS * j + i) * 4 + 2] = 0;
        bitmap[(PICS * j + i) * 4 + 3] = 0;
      }
      else
      {
        bitmap[(PICS * j + i) * 4] = 0;
        bitmap[(PICS * j + i) * 4 + 1] = 255 - min(20 * outside, 255);
        bitmap[(PICS * j + i) * 4 + 2] = 0;
        bitmap[(PICS * j + i) * 4 + 3] = 0;
        outside = 0;
      }
      pthread_mutex_unlock(&bitmap_mutex);
    }
    float ready = ceil(100.0f * i / PICS);
    printf("\b\b\b%3d", (int)ready);
  }
}


int generate(int noThreads)
{
  int i;
  generateThreadArg* arg;
  pthread_t thread1, thread2;
  
  // initialization of thread arguments
  arg = (generateThreadArg*) malloc(sizeof(generateThreadArg) * noThreads);
  for (i = 0; i < noThreads; i++)
  {
    arg[i].step = noThreads;
    arg[i].shift = i;
  }
  
  pthread_mutex_init(&bitmap_mutex, NULL);

  if (WRITECOORDS)
    printf("X : %f, Y : %f, zoom: %f\n", xpos, ypos, zoom2);
    
  printf("Generating picture -    %%");
  
  for (i = 0; i < noThreads; i++)
  {
    if (pthread_create(&thread1, NULL, generate_thread, (void*)&arg[i]))
    {
      fprintf(stderr, "Can't create thread #%d!\n", i);
      return -1;
    }
  }
  
  if (pthread_join(thread1, NULL))
  {
    fprintf(stderr, "Error while waiting for thread!\n");
    return -2;
  }
  
  pthread_mutex_destroy(&bitmap_mutex);
  
  printf("\n");
}


// draws image to screen
void drawBox()
{
  glDrawPixels(PICS, PICS, GL_RGBA, GL_UNSIGNED_BYTE, &bitmap);
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
  glutSwapBuffers();
  usleep(1);
}


void idle(void)
{
  display();
}


void keyPressed(unsigned char key, int i1, int i2)
{
  switch (key) {
  case 27:
    exit(0);
    break;
  case 'q':
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    glutSetOption(GLUT_ACTION_GLUTMAINLOOP_RETURNS, GLUT_ACTION_CONTINUE_EXECUTION);
    glutLeaveMainLoop();
    break;
  case 't':
    zoom2 *= 0.5;
    generate(NOTHREADS);
    break;
  case 'g':
    zoom2 /= 0.5;
    generate(NOTHREADS);
    break;
    
  case 'i':
    ypos -= zoom2 * STEPSIZE1;
    generate(NOTHREADS);
    break;
  case 'k':
    ypos += zoom2 * STEPSIZE1;
    generate(NOTHREADS);
    break;
  case 'j':
    xpos -= zoom2 * STEPSIZE1;
    generate(NOTHREADS);
    break;
  case 'l':
    xpos += zoom2 * STEPSIZE1;
    generate(NOTHREADS);
    break;
  case 'I':
    ypos -= zoom2 * STEPSIZE2;
    generate(NOTHREADS);
    break;
  case 'K':
    ypos += zoom2 * STEPSIZE2;
    generate(NOTHREADS);
    break;
  case 'J':
    xpos -= zoom2 * STEPSIZE2;
    generate(NOTHREADS);
    break;
  case 'L':
    xpos += zoom2 * STEPSIZE2;
    generate(NOTHREADS);
    break;
  case 'w':
    printf("X : %f, Y : %f, zoom: %f\n", xpos, ypos, zoom2);
    break;
  case 'p':
    writeBmp();
    break;
  }
}

int main(int argc, char **argv)
{
    int choose;
    
    // default value
    NOTHREADS = 1;
    
    printf("Melyik legyen?\n\n");
    printf("1 - Mandelbrot\n");
    printf("2 - Konjugalt Mandelbrot\n");
    printf("3 - Kobos\n");
    printf("4 - Konjugalt kobos\n");
    printf("5 - Kvartikus\n");
    printf("6 - Konjugalt kvartikus\n\n");
    printf("Valasztom: ");
    scanf("%d", &choose);
    
    switch(choose)
    {
      case 1:
        isOK = isOK_Mandelbrot;
        break;
      case 2:
        isOK = isOK_ConjMandelbrot;
        break;
      case 3:
        isOK = isOK_Cubic;
        break;
      case 4:
        isOK = isOK_ConjCubic;
        break;
      case 5:
        isOK = isOK_Quartic;
        break;
      case 6:
        isOK = isOK_ConjQuartic;
        break;
      default:
        printf("Hibas valasztas.\n\n");
        return -2;
    }
    
    if (readConfig() != 1)
      return -1;
    
    if (NOTHREADS > 4 || NOTHREADS < 1)
    {
      fprintf(stderr, "Invalid threadnumber!\n");
      return -3;
    }
    
    fileNumber = getFileNumber();
      
    generate(NOTHREADS);
   
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("first");
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

