
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "config.h"
#include "frac.h"


#define SIZE 0.001


//#define _height 450
//#define _width 450
//#define _pixelbytesize _height*_width*_bitsperpixel/8
//#define _filesize _pixelbytesize+sizeof(bitmap)
#define pixel 0xFF


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
    case 'W':
      if (!READONLY)
      {
        char ans;
        printf("Biztosan menteni akarod? [i/n]\nValasz: ");
        scanf("%c", &ans);
        switch(ans)
        {
          case 'i':
          case 'y':
          case 'I':
          case 'Y':
            if (writeConfig() == 1)
              printf("\nBeallitasok sikeresen mentve.\n");
            else
              fprintf(stderr, "\nHiba mentes kozben!\n");
            break;
          case 'n':
          case 'N':
            break;
          default:
            printf("\nRossz valasz.\n");
            break;
        }
      }
      else
        printf("\nA konfiguracios file irasvedett.\n");
      break;
    case 'p':
      writeBmp();
      break;
  }
}


int main(int argc, char **argv)
{   
    bool hasChosenFractal = false;
    bool isGcxSet = false;
    bool isGcySet = false;
    bool printOnly = false;
    int i; 
    
    // default values
    NOTHREADS = 1;
    gcx = 0.0;
    gcy = 0.0;
        
    if (readConfig() != 1)
      return -1;
    
    if (NOTHREADS > 4 || NOTHREADS < 1)
    {
      fprintf(stderr, "Invalid threadnumber!\n");
      return -3;
    }
    
    // processing command-line arguments
    int firstChar = 0;
    if (HYPHENNEEDED)
      firstChar = 1;
    
    for (i = 1; i < argc; i++)
    {
      float temp;
      
      if (HYPHENNEEDED && argv[i][0] != '-')
      {
        fprintf(stderr, "Hyphen is needed on the beginning of argument!\n");
        return -4;
      }
      
      switch(argv[i][firstChar])
      {
        case 'f':
          if(sscanf(&argv[i][firstChar + 1], "%d", &chosenFractal) != 1)
          {
            fprintf(stderr, "Couldn't process command-line argument: \'x\' !");
            return -4;
          }
          hasChosenFractal = true;
          break;
        case 'x':
          if(sscanf(&argv[i][firstChar + 1], "%f", &xpos) != 1)
          {
            fprintf(stderr, "Couldn't process command-line argument: \'x\' !");
            return -4;
          }
          break;
        case 'y':
          if(sscanf(&argv[i][firstChar + 1], "%f", &ypos) != 1)
          {
            fprintf(stderr, "Couldn't process command-line argument: \'y\' !");
            return -4;
          }
          break;
        case 'z':
          if(sscanf(&argv[i][firstChar + 1], "%f", &zoom2) != 1)
          {
            fprintf(stderr, "Couldn't process command-line argument: \'z\' !");
            return -4;
          }
          break;
        case 'p':
          printOnly = true;
          break;
        case 't':
          if(sscanf(&argv[i][firstChar + 1], "%d", &NOTHREADS) != 1)
          {
            fprintf(stderr, "Couldn't process command-line argument: \'t\' !");
            return -4;
          }
          break; 
        case 'c':
          switch(argv[i][firstChar + 1])
          {
            case 'x':
              if(sscanf(&argv[i][firstChar + 2], "%f", &gcx) != 1)
              {
                fprintf(stderr, "Couldn't process command-line argument: \'cx\' !");
                return -4;
              }
              isGcxSet = true;
              break;
            case 'y':
              if(sscanf(&argv[i][firstChar + 2], "%f", &gcy) != 1)
              {
                fprintf(stderr, "Couldn't process command-line argument: \'cy\' !");
                return -4;
              }
              isGcySet = true;
              break;
            default:
              fprintf(stderr, "Invalid command-line parameter: \'%c%c\' !",
                      argv[i][firstChar], argv[i][firstChar + 1]);
              return -4;
          }
        default:
          fprintf(stderr, "Invalid command-line parameter: \'%c\' !\n", argv[i][0]);
          return -4;
      }
    }
    
    if (!hasChosenFractal)
    {
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
    }
      
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
        if (!isGcxSet || !isGcySet)
          printf("Komplex parameter erteke(i):\n");
        if (!isGcxSet)
        {
          printf("cx = ");
          scanf("%f", &gcx);
        }
        if (!isGcySet)
        {
          printf("cy = ");
          scanf("%f", &gcy);
        }
        break;
      case 8:
        isOK = isOK_ConjJulia;
        if (!isGcxSet || !isGcySet)
          printf("Komplex parameter ertekei:\n");
        if (!isGcxSet)
        {
          printf("cx = ");
          scanf("%f", &gcx);
        }
        if (!isGcySet)
        {
          printf("cy = ");
          scanf("%f", &gcy);
        }
        break;
      default:
        printf("Hibas valasztas.\n\n");
        return -2;
    }
    
    fileNumber = getFileNumber();
    
    bitmap = (char*) malloc(HEIGHT * WIDTH * 4 * sizeof(char));
      
    generate(NOTHREADS);
    
    if(printOnly)
    {
      writeBmp();
      return 1;
    }
   
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("2D fractal viewer");
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

