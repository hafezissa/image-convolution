#ifndef CONV_H_
#define CONV_H_

#define MAX(x,y) (x>=y ? y : x)
#define MIN(x,y) (x<=y ? y : x)
#define RGB_COMP 255

typedef struct {
    unsigned int R,G,B;
} Pixel;

typedef struct {
    int len, wid;
    Pixel *img_data;
} PPMImage;

typedef struct {
    int n;
    int scale;
    int *matrix;
} Kernel;

#endif