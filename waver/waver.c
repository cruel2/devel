
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
#include <stdlib.h> // for malloc
#include "waver.h"
#include "make_wav.h"
 
#define S_RATE  (44100)
#define BUF_SIZE (S_RATE*4) /* 4 second buffer */
 
 
//int buffer[BUF_SIZE];
int* buffer;
int bufferSize;

int noInstruments;
int noNotes;
note n[10];
float scale[12];
instrument instr[2];

// general settings
float VOLUME;
 
 
int readConfig(char* fn)
{
  char line[20];
  
  printf("Reading config file.\n");
  
  FILE* fp = fopen(fn, "r");
  
  if (!fp)
  {
    printf("Can't open tab file!\n");
    return -1;
  }
  
  while(fscanf(fp, "%s\n", &line[0]) != EOF)
  {
    if (strncmp("VOLUME", &line[0], 6) == 0)
    {
      sscanf(&line[7], "%f", &VOLUME);
      continue;
    }
  }
  
  fclose(fp);
  
  printf("Config read succeeded.\n");
  
  return 1;
}
 

int readTab(char* fn)
{
  char line[20];
  enum blockState bState = NONE;
  int instrCounter = 0;
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
    
    if (strncmp("[instrument]", &line[0], 12) == 0)
    {
      bState = INSTRUMENT;
      //printf("instrument!\n");
      continue;
    }
    
    if (strncmp("[end]", &line[0], 5) == 0)
    {
      switch (bState)
      {
        case SOUND:
          noteCounter++;
          break;
        case INSTRUMENT:
          instrCounter++;
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
    
    if (strncmp("length", &line[0], 6) == 0 && bState == SOUND)
    {
      float t;
      sscanf(&line[7], "%f", &t);
      n[noteCounter].length = t;
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
    
    if (strncmp("freq", &line[0], 4) == 0 && bState == SOUND)
    {
      int t;
      sscanf(&line[5], "%d", &t);
      n[noteCounter].freq = 440.0 * scale[t];
      printf("freq: %f\n", n[noteCounter].freq);
      continue;
    }
    
    if (strncmp("harmonics", &line[0], 9) == 0 && bState == INSTRUMENT)
    {
      int t, i;
      sscanf(&line[10], "%d", &t);
      instr[instrCounter].noHarmonics = t;
      instr[instrCounter].harmonic = (float*) malloc(sizeof(float) * (t+1));
      for (i = 0; i <= t; i++)
      {
        fscanf(fp, "%f\n", &(instr[instrCounter].harmonic[i]));
        //printf("#%d : %f\n", i, instr[instrCounter].harmonic[i]);
      }
      //printf("freq: %f\n", n[noteCounter].freq);
      continue;
    }
  }
  
  fclose(fp);
  
  printf("Instruments read: %d\n", instrCounter);
  printf("Notes read: %d\n\n", noteCounter);
  noInstruments = instrCounter;
  noNotes = noteCounter;
  
  return 1;
}


float processTab()
{
  int i, j, k;
  
  float freq = 440; // frequency in Hz
  //float phaseStep = freq * 2.0 * M_PI / S_RATE;
  float phase[10];
  float time = 0.0;
  
  // TODO : okosabban!
  for (i = 0; i < 10; i++)
    phase[i] = 0.0;
  
  for (i = 0; i<bufferSize; i++)
  {
    buffer[i] = 0.0;
    
    for (j = 0; j < noNotes; j++)
    {
      if (n[j].begin < time && time < n[j].begin + n[j].length)
      {
        float temp = 0.0;
        
        for (k = 0; k < instr[0].noHarmonics; k++)
        {
          phase[k] += n[j].freq * 2.0 * M_PI / S_RATE * (k+1);
          temp += sin(phase[k]) * instr[0].harmonic[k];
        }
        
        buffer[i] += (int) (5000.0 * temp); // TODO dummy amplitude
      }
    }
    
    time += 1.0 / S_RATE;
  }
}

 
float getTabLength(/*tab* t*/)
{
  int i;
  float res = 0.0;
  
  /*for (i = 0; i < t->noNotes; i++)
  {
    if (t->n[i].begin + t->n[i].length > res)
      res = t->n[i].begin + t->n[i].length;
  }*/
  
  for (i = 0; i < noNotes; i++)
  {
    if (n[i].begin + n[i].length > res)
      res = n[i].begin + n[i].length;
  }
  
  return res;
}


void init()
{
  int i;
  
  // fills the chromatic scale's frequency multipliers
  for (i = 0; i < 12; i++)
    scale[i] = pow(2.0, i / 12.0);
}


void destroy()
{
  int i;
  
  for (i = 0; i < noInstruments; i++)
    free(instr[i].harmonic);
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
    VOLUME = 0.0;
    
    init();
    
    if (readConfig("config.ini") != 1)
    {
      fprintf(stderr, "Couldn't read config file!\n");
      return -1;
    }
    
    if (readTab("test.tab") != 1)
    {
      fprintf(stderr, "Couldn't read tab file!\n");
      return -1;
    }
    
    printf("length: %f\n", getTabLength());
    
    // TODO : miert kell ide a 2-es szorzo?
    bufferSize = 2*(int)ceil(S_RATE * getTabLength());
    buffer = (int*)calloc(sizeof(int), bufferSize);
    
    processTab();
 
    write_wav("test.wav", bufferSize, buffer, S_RATE);
    
    free(buffer);
    
    destroy();
 
    return 0;
}

