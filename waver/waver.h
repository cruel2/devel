
#ifndef WAVER_H
#define WAVER_H


typedef struct
{
  float volume;
} mainSettings;

typedef struct
{
  int noHarmonics;
  float* freq;
  // float* phase;  // maybe it isn't needed
} instrument;

typedef struct
{
  float begin, end;
  int instr;
  //float volume; // TODO
} note;

// kesobb tobb savura kell atirni
typedef struct
{
  note* n;
  int noNotes;
} tab;

enum blockState {NONE, SOUND};

#endif

