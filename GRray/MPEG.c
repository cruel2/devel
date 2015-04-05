#define UINT64_C long // FIXME!

#define PI 3.1415927

#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <string.h>

#include "particle.h"

#ifdef HAVE_AV_CONFIG_H
#undef HAVE_AV_CONFIG_H
#endif

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"
}

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096


// for the picture rendering
//int height, width;
char** bitmaps1;
unsigned char** bitmaps2;
unsigned char** bitmaps3;
//char* emptyBitmap; // the empty memory area
//char* bitmap1;     // for the U component
//char* bitmap2;     // for the V component
char* bitmap;


float max(float a, float b)
{
  if (a >= b)
    return a;
  else
    return b;
}


/*
 * Video encoding example
 */
static void video_encode_example(const char *filename)
{
    AVCodec *codec;
    AVCodecContext *c= NULL;
    int i, out_size, size, x, y, outbuf_size;
    FILE *f;
    AVFrame *picture;
    uint8_t *outbuf, *picture_buf;

    printf("Video encoding\n");

    /* find the mpeg1 video encoder */
    codec = avcodec_find_encoder(CODEC_ID_MPEG1VIDEO);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    c= avcodec_alloc_context3(codec);
    picture= avcodec_alloc_frame();

    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = WIDTH;
    c->height = HEIGHT;
    /* frames per second */
    c->time_base= (AVRational){1,25};
    c->gop_size = 10; /* emit one intra frame every ten frames */
    c->max_b_frames=1;
    c->pix_fmt = PIX_FMT_YUV420P;

    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }

    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "could not open %s\n", filename);
        exit(1);
    }

    /* alloc image and output buffer */
    outbuf_size = 100000;
    outbuf = (uint8_t*)malloc(outbuf_size);
    size = c->width * c->height;
    picture_buf = (uint8_t*)malloc((size * 3) / 2); /* size for YUV 420 */

    picture->data[0] = picture_buf;
    picture->data[1] = picture->data[0] + size;
    picture->data[2] = picture->data[1] + size / 4;
    picture->linesize[0] = c->width;
    picture->linesize[1] = c->width / 2;
    picture->linesize[2] = c->width / 2;

    /* encode 1 second of video */
    for(i=0;i<NO_BMPS;i++) {
        fflush(stdout);
        //memcpy(bitmap1, emptyBitmap, HEIGHT * WIDTH);
        //memcpy(bitmap2, emptyBitmap, HEIGHT * WIDTH);
        picture->data[0] = (uint8_t*)bitmaps1[i];
        picture->data[1] = (uint8_t*)bitmaps2[i];
        picture->data[2] = (uint8_t*)bitmaps3[i];

        /* encode the image */
        out_size = avcodec_encode_video(c, outbuf, outbuf_size, picture);
        printf("encoding frame %3d (size=%5d)\n", i, out_size);
        fwrite(outbuf, 1, out_size, f);
    }

    /* get the delayed frames */
    for(; out_size; i++) {
        fflush(stdout);

        out_size = avcodec_encode_video(c, outbuf, outbuf_size, NULL);
        printf("write frame %3d (size=%5d)\n", i, out_size);
        fwrite(outbuf, 1, out_size, f);
    }

    /* add sequence end code to have a real mpeg file */
    outbuf[0] = 0x00;
    outbuf[1] = 0x00;
    outbuf[2] = 0x01;
    outbuf[3] = 0xb7;
    fwrite(outbuf, 1, 4, f);
    fclose(f);
    free(picture_buf);
    free(outbuf);

    avcodec_close(c);
    av_free(c);
    av_free(picture);
    printf("\n");
}




/*************************************************
    Simulation and other data initialization
*************************************************/

void init()
{
   
  bitmap = (char*) malloc(HEIGHT * WIDTH * 4 * sizeof(char));
  readBmp();
  
  CENTERX = -0.5;
  CENTERY = -0.5;
  
  
    //height = 400;
    //width  = 400;
    //emptyBitmap = (char*) calloc(HEIGHT * WIDTH, sizeof(char));
    //bitmap1 = (char*) calloc(HEIGHT * WIDTH, sizeof(char));
    //bitmap2 = (char*) calloc(HEIGHT * WIDTH, sizeof(char));
    bitmaps1 = (char**) malloc(NO_BMPS * sizeof(char**));
    bitmaps2 = (unsigned char**) malloc(NO_BMPS * sizeof(char**));
    bitmaps3 = (unsigned char**) malloc(NO_BMPS * sizeof(char**));
    for (int i = 0; i < NO_BMPS; i++)
    {
        bitmaps1[i] = (char*) malloc(HEIGHT * WIDTH * sizeof(char));
        bitmaps2[i] = (unsigned char*) malloc(HEIGHT * WIDTH * sizeof(char));
        bitmaps3[i] = (unsigned char*) malloc(HEIGHT * WIDTH * sizeof(char));
    }
        
    //for (int i = 0; i < HEIGHT * WIDTH; i++)
    //    emptyBitmap[i] = 128;
        
    /* must be called before using avcodec lib */
    //avcodec_init();

    /* register all the codecs */
    avcodec_register_all();
}


void finish()
{
}

// the size of a planet on the visualization
#define SQ_SIZE 0.005f

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    glutSwapBuffers();   
}


void render()
{
    static long bmp_counter = 0;
    
    generate();
    CENTERX += 0.01;
    CENTERY += 0.01;
    
    if (bmp_counter == NO_BMPS)
    {
        video_encode_example("1.mpg");
        //bmp_counter++;
        printf("Kilepes\a\n");
        exit(1);
    }
    if (bmp_counter >= NO_BMPS)
        return;        
        
    for (int i = 0; i < WIDTH; i++)
    {
      
      for (int j = 0; j < HEIGHT; j++)
      {  
          
        int idx1y = (i +         j * WIDTH);
        int idx2 =  (i + (HEIGHT-j) * WIDTH);
        int r = bitmap[idx2 * 4];
        int g = bitmap[idx2 * 4 + 1];
        int b = bitmap[idx2 * 4 + 2];
        if (r < 0)
          r = 256 + r;
        if (g < 0)
          g = 256 + g;
        if (b < 0)
          b = 256 + b;
        
        int y = (( 66 * r + 129 * g +  25 * b + 128)>>8);
        int u = ((-38 * r -  74 * g + 112 * b + 128)>>8) + 128;
        int v = ((112 * r -  94 * g -  18 * b + 128)>>8) + 128;
        
        bitmaps1[bmp_counter][idx1y] = (char)y;
        
        if (j % 2 == 0)
        {
          int idx1u = (i + j/2 * WIDTH) / 2;

          bitmaps2[bmp_counter][idx1u] = (char)u;
          bitmaps3[bmp_counter][idx1u] = (char)v;
        }
        //if (u != 128)
          //printf("DEBUG y u v : %i  %i  %i\n", y, u, v);
      }
    }    
        
    bmp_counter++;
    
    if (bmp_counter % 1 == 0)
        printf("Frame nr. %i / %i\n", bmp_counter, NO_BMPS);
}


void idle(void)
{
    static long counter = 0;
    
    render();
    
    counter++;
}


void keyPressed(unsigned char key, int, int)
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
  }
}


int main(int argc, char** argv)
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Line");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed); // TODO : with keyboard too
    glutIdleFunc(idle); 
    glutMainLoop();
    
    finish();

}
