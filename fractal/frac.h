
#ifndef FRAC_H
#define FRAC_H

#include <stdint.h>

#define X_TRANS 0.5

// bool type
#define bool int
#define true 1
#define false 0

// constants for bmp header
#define _planes 1
#define _compression 0
#define _xpixelpermeter 0x130B //2835 , 72 DPI
#define _ypixelpermeter 0x130B //2835 , 72 DPI

// structs for bmp file
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

//this structure contains the parameters for the fractal generating threads
typedef struct
{
  int shift;    // number of thread minus one
  int step;     // number of threads
} generateThreadArg;


extern char* bitmap;
extern int fileNumber;

extern float xpos;
extern float ypos;
extern float zoom2;

// complex parameter for the chosen fractal
extern float gcx, gcy;


bool isOK_Mandelbrot(int xx, int yy, int* outside);
bool isOK_ConjMandelbrot(int xx, int yy, int* outside);
bool isOK_Cubic(int xx, int yy, int* outside);
bool isOK_ConjCubic(int xx, int yy, int* outside);
bool isOK_Quartic(int xx, int yy, int* outside);
bool isOK_ConjQuartic(int xx, int yy, int* outside);
bool isOK_Julia(int xx, int yy, int* outside);
bool isOK_ConjJulia(int xx, int yy, int* outside);

int getFileNumber();

#endif

