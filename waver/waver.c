
/*

includes part from:

http://gasstationwithoutpumps.wordpress.com/2011/10/08/making-wav-files-from-c-programs/

*/

/* test_make_wav.c
 * Fri Jun 18 17:13:19 PDT 2010 Kevin Karplus
 * Test program for the write_wav function in make_wav.c
 */
 
#include <math.h>
#include <stdio.h>
#include "waver.h"
#include "make_wav.h"
 
#define S_RATE  (44100)
#define BUF_SIZE (S_RATE*2) /* 2 second buffer */
 
 
int buffer[BUF_SIZE];
int noNotes;
note n[10];
 

int readTab(char* fn)
{
  char line[20];
  enum blockState bState = NONE;
  int noteCounter = 0;
  
  printf("\nReading tab file\n");
  
  FILE* fp = fopen(fn, "r");
  
  if (!fp)
  {
    printf("Can't open tab file!\n");
    return -1;
  }
  
  while(fscanf(fp, "%s\n", &line[0]) != EOF)
  {
    //printf("line: %s\n", line);
    
    if (strncmp("[sound]", &line[0], 7) == 0)
    {
      bState = SOUND;
      //printf("sound!\n");
      continue;
    }
    
    if (strncmp("[end]", &line[0], 5) == 0)
    {
      switch (bState)
      {
        case SOUND:
          noteCounter++;
          break;
        default:
          fprintf(stderr, "Error during reading tab file!\n");
          fclose(fp);
          return -2;
      }
      bState = NONE;
      //printf("none!\n");
      continue;
    }
    
    if (strncmp("begin", &line[0], 5) == 0 && bState == SOUND)
    {
      float t;
      sscanf(&line[6], "%f", &t);
      n[noteCounter].begin = t;
      //printf("begin: %f\n", t);
      continue;
    }
    
    if (strncmp("end", &line[0], 3) == 0 && bState == SOUND)
    {
      float t;
      sscanf(&line[4], "%f", &t);
      n[noteCounter].end = t;
      //printf("end: %f\n", t);
      continue;
    }
    
    if (strncmp("instr", &line[0], 5) == 0 && bState == SOUND)
    {
      int t;
      sscanf(&line[6], "%d", &t);
      n[noteCounter].instr = t;
      //printf("instr: %d\n", t);
      continue;
    }
  }
  
  fclose(fp);
  
  printf("Notes read: %d\n\n", noteCounter);
  noNotes = noteCounter;
  
  return 1;
}


float processTab()
{
  int i, j;
  
  float freq = 440; // frequency in Hz
  float phaseStep = freq * 2.0 * M_PI / S_RATE;
  float phase = 0.0;
  float time = 0.0;
  
  for (i = 0; i<BUF_SIZE; i++)
  {
    buffer[i] = 0.0;
    
    for (j = 0; j < noNotes; j++)
    {
      if (n[j].begin < time && time < n[j].end)
      {
        buffer[i] += (int) (sin(phase) * 10000); // TODO dummy amplitude
      }
    }
    
    phase += phaseStep;
    time += 1.0 / S_RATE;
  }
}

 
float getTabLength(tab* t)
{
  int i;
  float res = 0.0;
  
  for (i = 0; i < t->noNotes; i++)
  {
    if (t->n[i].end > res)
      res = t->n[i].end;
  }
  
  return res;
} 
 
 
int main(int argc, char ** argv)
{
    int i;
    float t;
    float amplitude = 16000;
    float freq_Hz = 440;
    float phase1 = 0.0;
    float phase2 = 0.0;
    float phase3 = 0.0;
    
    // initialization for global variables
    noNotes = 0;
    
    // testing new functions
    if (readTab("test.tab") != 1)
    {
      fprintf(stderr, "Couldn't read tab file!\n");
      return -1;
    }
    
    processTab();
 
    // frequency: radians per sampling rate
    /*float freq1 = freq_Hz*2*M_PI/S_RATE;
    float freq2 = freq1 * 1.5;
    float freq3 = freq1 * 2.0;*/
 
    /* fill buffer with a sine wave */
    /*for (i=0; i<BUF_SIZE; i++)
    {
        if (i < BUF_SIZE/3)
        {
            phase1 += freq1;
            phase2 += freq2;
            phase3 += freq3;
            
            buffer[i] = (int)(amplitude * 
                (sin(phase1) * 1.0 + 
                 sin(phase2) * 0.7 + 
                 sin(phase3) * 0.4 ));
        }
        else if (i < 2*BUF_SIZE/3)
        {
            phase1 += freq1;
            phase2 += freq2;
            phase3 += freq3;
            
            buffer[i] = (int)(amplitude * 
                (sin(phase1) * 0.0 + 
                 sin(phase2) * 0.7 + 
                 sin(phase3) * 0.4 ));
        }
        else buffer[i] = 0;
    }*/
 
    write_wav("test.wav", BUF_SIZE, buffer, S_RATE);
 
    return 0;
}

