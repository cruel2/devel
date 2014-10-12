
#ifndef WAVER_H
#define WAVER_H


typedef struct
{
  float volume;
} mainSettings;

typedef struct
{
  float begin, length;
  int instr;
  float freq;
  //float volume; // TODO
} note;

typedef struct
{
  int noHarmonics;
  float* harmonic;
} instrument;

// kesobb tobb savura kell atirni
typedef struct
{
  note* n;
  int noNotes;
} tab;

enum blockState {NONE, SOUND, INSTRUMENT};

#endif

