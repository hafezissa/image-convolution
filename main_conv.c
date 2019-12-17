#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conv.h"


PPMImage* readImage(const char *file);
Kernel* readKernel(const char *file);
PPMImage* convolution(PPMImage *image, Kernel* kernel);


int main(int argc, char** argv)
{
    if (argc != 4) {
        printf("Usage: ./filter input.ppm  kernel output.ppm\n");
        return -1;
    }

    char* file_in = argv[1];
    char* file_out = argv[3];
    char* file_kernel = argv[2];

    PPMImage* image;
    image = readImage(file_in);
    Kernel* kernel;
    kernel = readKernel(file_kernel);
    PPMImage *imageOUT = convolution(image, kernel);
    writePPM(file_out, imageOUT);

}