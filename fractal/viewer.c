
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "config.h"
#include "frac.h"


#define SIZE 0.001


#define _height 450
#define _width 450
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


// this is the number of the latest picture in the pictures' folder
int fileNumber;

int chosenFractal;

// complex parameter for the chosen fractal
float gcx, gcy;

int outside = 0;
char* bitmap;
pthread_mutex_t bitmap_mutex;

// Points to the inclusion function
bool (*isOK)(int, int, int*);


inline unsigned char min(int a, int b)
{
    return (unsigned char) (a > b? b:a);
}


// it will free the allocated resources and does some exit stuff
void destroy()
{
  printf("\n");
  free(bitmap);
}


void writeBmp()
{
  char fileName[13];
  char* myBitmap;
  char* oldBitmap;
  int myFileSize;
  int oldWidth, oldHeight;
  
  if (PRINTDOUBLE)
  {
    oldBitmap = bitmap;
    myBitmap = (char*) malloc (HEIGHT*WIDTH*4*4);
    bitmap = myBitmap;
    generate();
  }
  else
    myBitmap = bitmap;
  
  fileNumber++;
  strncpy(fileName, "test", 4);
  sprintf(fileName + 4, "%4.4d", fileNumber);
  strncpy(fileName + 8, ".bmp", 4);
  
  oldWidth = WIDTH;
  oldHeight = HEIGHT;
  
  if (PRINTDOUBLE)
  {
    WIDTH *= 2;
    HEIGHT *= 2;
  }
  
  myFileSize = WIDTH * HEIGHT * 4;
      
  FILE *fp = fopen(fileName,"wb");
  bitmap_type *pbitmap  = (bitmap_type*)calloc(1,sizeof(bitmap_type));
  uint8_t *pixelbuffer = (uint8_t*)(myBitmap);
  pbitmap->fileheader.signature[0] = 'B';
  pbitmap->fileheader.signature[1] = 'M';
  pbitmap->fileheader.filesize = myFileSize;
  pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap_type);
  pbitmap->bitmapinfoheader.dibheadersize = sizeof(bitmapinfoheader_type);
  pbitmap->bitmapinfoheader.width = WIDTH;
  pbitmap->bitmapinfoheader.height = HEIGHT;
  pbitmap->bitmapinfoheader.planes = _planes;
  pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
  pbitmap->bitmapinfoheader.compression = _compression;
  pbitmap->bitmapinfoheader.imagesize = myFileSize;
  pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter ;
  pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter ;
  pbitmap->bitmapinfoheader.numcolorspallette = 0;
  fwrite (pbitmap, 1, sizeof(bitmap_type),fp);
  
  fwrite(pixelbuffer, 1, myFileSize, fp);
  fclose(fp);
  free(pbitmap);
  
  if (PRINTDOUBLE)
  {
    WIDTH /= 2;
    HEIGHT /= 2;
    myFileSize = WIDTH * HEIGHT * 4;
    bitmap = oldBitmap;
    free(myBitmap);
  }
}


void* generate_thread_Mandelbrot(void* arg_)
{
  int i, j, ii, jj;
  int outside;
  generateThreadArg* arg = (generateThreadArg*) arg_;
  
  for (i = arg->shift; i < WIDTH; i+=arg->step)
  {
    for (j = 0; j < HEIGHT; j++)
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
        bitmap[(WIDTH * j + i) * 4] = 15 * sum;
        bitmap[(WIDTH * j + i) * 4 + 1] = 0;
        bitmap[(WIDTH * j + i) * 4 + 2] = 0;
        bitmap[(WIDTH * j + i) * 4 + 3] = 0;
      }
      else
      {
        bitmap[(WIDTH * j + i) * 4] = 0;
        bitmap[(WIDTH * j + i) * 4 + 1] = 255 - min(20 * outside, 255);
        bitmap[(WIDTH * j + i) * 4 + 2] = 0;
        bitmap[(WIDTH * j + i) * 4 + 3] = 0;
        outside = 0;
      }
      pthread_mutex_unlock(&bitmap_mutex);
    }
    float ready = ceil(100.0f * i / WIDTH);
    printf("\b\b\b%3d", (int)ready);
  }
}


void* generate_thread_Julia(void* arg_)
{
  int i, j, ii, jj;
  int outside;
  generateThreadArg* arg = (generateThreadArg*) arg_;
  
  for (i = arg->shift; i < WIDTH; i+=arg->step)
  {
    for (j = 0; j < HEIGHT; j++)
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
        bitmap[(WIDTH * j + i) * 4] = 15 * sum;
        bitmap[(WIDTH * j + i) * 4 + 1] = 0;
        bitmap[(WIDTH * j + i) * 4 + 2] = 0;
        bitmap[(WIDTH * j + i) * 4 + 3] = 0;
      }
      else
      {
        if (outside < 150)
        {
          bitmap[(WIDTH * j + i) * 4] = min(2 * outside, 255);
          bitmap[(WIDTH * j + i) * 4 + 1] = 0;
          bitmap[(WIDTH * j + i) * 4 + 2] = 0;
          bitmap[(WIDTH * j + i) * 4 + 3] = 0;
        }
        else
        {
          bitmap[(WIDTH * j + i) * 4] = 255;
          bitmap[(WIDTH * j + i) * 4 + 1] = min(2 * (outside - 150), 255);
          bitmap[(WIDTH * j + i) * 4 + 2] = 0;
          bitmap[(WIDTH * j + i) * 4 + 3] = 0;
        }
        outside = 0;
      }
      pthread_mutex_unlock(&bitmap_mutex);
    }
    float ready = ceil(100.0f * i / WIDTH);
    printf("\b\b\b%3d", (int)ready);
  }
}



int generate(int noThreads)
{
  int i;
  generateThreadArg* arg;
  pthread_t thread1, thread2;
  void* (*genThread)(void*);
  
  // initialization of thread arguments
  arg = (generateThreadArg*) malloc(sizeof(generateThreadArg) * noThreads);
  for (i = 0; i < noThreads; i++)
  {
    arg[i].step = noThreads;
    arg[i].shift = i;
  }
  
  pthread_mutex_init(&bitmap_mutex, NULL);

  if (WRITECOORDS)
    printf("\nX : %f, Y : %f, zoom: %f\n", xpos, ypos, zoom2);
    
  printf("Generating picture -    %%");
  
  switch(chosenFractal)
  {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      genThread = generate_thread_Mandelbrot;
      break;
    case 7:
    case 8:
      genThread = generate_thread_Julia;
      break;
  }
      
  for (i = 0; i < noThreads; i++)
  {
    if (pthread_create(&thread1, NULL, genThread, (void*)&arg[i]))
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
  display();
}


void keyPressed(unsigned char key, int i1, int i2)
{
  switch (key) {
    case 27:
    case 'q':
      destroy();
      exit(0);
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
    int i; 
    
    // default value
    NOTHREADS = 1;
    
    printf("Melyik legyen?\n\n");
    printf("1 - Mandelbrot\n");
    printf("2 - Konjugalt Mandelbrot\n");
    printf("3 - Kobos\n");
    printf("4 - Konjugalt kobos\n");
    printf("5 - Kvartikus\n");
    printf("6 - Konjugalt kvartikus\n");
    printf("7 - Julia\n");
    printf("8 - Konjugalt Julia\n\n");
    printf("Valasztom: ");
    scanf("%d", &chosenFractal);
    
    switch(chosenFractal)
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
      case 7:
        isOK = isOK_Julia;
        printf("Komplex parameter ertekei:\n");
        printf("cx = ");
        scanf("%f", &gcx);
        printf("cy = ");
        scanf("%f", &gcy);
        break;
      case 8:
        isOK = isOK_ConjJulia;
        printf("Komplex parameter ertekei:\n");
        printf("cx = ");
        scanf("%f", &gcx);
        printf("cy = ");
        scanf("%f", &gcy);
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
    
    // processing command-line arguments
    for (i = 1; i < argc; i++)
    {
      switch(argv[i][0])
      {
        case 'f':
          break;
        case 'x':
          //float temp;
          //sscanf()
          break;
        case 'y':
          break;
        case 'z':
          break;
        case 'p':
          break;
        case 't':
          break; 
        default:
          printf("Invalid command-line parameter: \'%c\' !", argv[i][0]);
          return -4;
      }
    }
    
    fileNumber = getFileNumber();
    
    bitmap = (char*) malloc(HEIGHT * WIDTH * 4 * sizeof(char));
      
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

