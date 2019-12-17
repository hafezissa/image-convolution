#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "conv.h"

PPMImage* readImage(const char *file)
{
    char line_buff[20];

    FILE *fp;
    PPMImage *image;

    fp = fopen(file, "rb");
    if (!fp)
    {
        printf("Can't open %s\n", file);
        exit(0);
    }

    fgets(line_buff, 20, fp); 
    if ('P' != line_buff[0] || '3' != line_buff[1])
    {
        printf("Invalid format\n");
        exit(0);
    }

    image = (PPMImage*) malloc(sizeof(PPMImage));
    if (!image)
    {
        printf("Failed to allocate mem\n");
        exit(0);
    }

    if (2 != fscanf(fp, "%d %d", &image->len, &image->wid))
    {
        printf("Incorrect image size\n");
        exit(0);
    }

    int rgb_component;

    if (1 != fscanf(fp,"%d", &rgb_component))
    {
        printf("Incorrect RGB component\n");
        exit(0);
    }

    if (RGB_COMP != rgb_component)
    {
        printf("Incorrect colour range\n");
        exit(0);
    }

    while('\n' != fgetc(fp));

    image->img_data = (Pixel*) malloc(image->len * image->wid * sizeof(Pixel));

    if (!image)
    {
        printf("Failed to allocate mem\n");
        exit(0);
    }

    int i = 0;
    int c, r,g,b;
    while (i < image->len * image->wid)
    {
        if ((c = fgetc(fp)) != EOF)
        {
            ungetc(c, fp);
            if (fscanf(fp, "%d %d %d", &r, &g, &b) == 3)
            {
                (image->img_data + i)->R = r;
                (image->img_data + i)->G = g;
                (image->img_data + i)->B = b;
            }
            i++;
        }
    }

    fclose(fp);
    return(image);
}

Kernel* readKernel(const char *file)
{

    FILE *fp;
    Kernel *kernel;
    kernel = (Kernel*) malloc(sizeof(Kernel));
    fp = fopen(file, "rb");

    if (!fp)
    {
        printf("Can't open %s\n", file);
        exit(1);
    }

    if (1 != fscanf(fp, "%d", &kernel->n))
    {
        printf("Failed to assign n :: Kernel\n");
        exit(1);
    }

    if (1 != fscanf(fp, "%d", &kernel->scale))
    {
        printf("Failed to assign scale :: Kernel\n");
        exit(1);
    }

    while('\n' != fgetc(fp));

    int i = 0;
    kernel->matrix = (int*) malloc(kernel->n * kernel->n * sizeof(int));
    while (fscanf(fp, "%d", &(kernel->matrix[i])) != EOF)
        ++i;    
    
    fclose(fp);
    return(kernel);
    
}

PPMImage* convolution(PPMImage *image, Kernel *kernel)
{
    int img_i,img_j,mtrx_k,mtrx_l;
    int mtrx_dim = kernel->n;

    int img_rows = image->wid, img_col = image->len;
    /* new image */
    PPMImage* image_out;
    image_out = (PPMImage*) malloc(sizeof(PPMImage));
    image_out->len = img_col;
    image_out->wid = img_rows;
    image_out->img_data = (Pixel*) malloc(img_rows * img_col * sizeof(Pixel));

    int i = 0;
    int matrix[kernel->n*kernel->n];
    while (i<kernel->n*kernel->n)
    {
        matrix[i] = kernel->matrix[i];
        ++i;
    }

    /* over image rows */
    for (img_i = 0; img_i<img_rows; img_i++)
    {
        /* over image columns */
        for (img_j = 0; img_j<img_col; img_j++)
        {            
            float sum_r = 0, sum_g = 0, sum_b = 0;
            for (mtrx_k = 0; mtrx_k<mtrx_dim; mtrx_k++) 
            {
                for (mtrx_l = 0; mtrx_l<mtrx_dim; mtrx_l++)
                {
                    int row = (img_i + mtrx_k);
                    int col = (img_j + mtrx_l);

                    /* edge cases (?) */
                    row = (row < 0) ? 0 : row;
                    col = (col < 0) ? 0 : col;
                    row = (row >= img_rows) ? img_rows-1 : row;
                    col = (col >= img_col) ? img_col-1 : col;

                    Pixel* pixel = (image->img_data + (row*img_col+col));

                    int pixel_r = pixel->R;
                    int pixel_g = pixel->G;
                    int pixel_b = pixel->B;

                    int krnl_whole = matrix[mtrx_k*mtrx_dim+mtrx_l];
                    float krnl_val = (float)krnl_whole/kernel->scale;

                    sum_r += pixel_r * krnl_val;
                    sum_b += pixel_b * krnl_val;
                    sum_g += pixel_g * krnl_val;
                }
            }
            /* MACRO for max and min */
            sum_r = MAX(sum_r,255), sum_r = MIN(sum_r,0);
            (image_out->img_data + (img_i*img_col+img_j))->R = (int)sum_r;
            sum_g = MAX(sum_g,255), sum_g = MIN(sum_g,0);
            (image_out->img_data + (img_i*img_col+img_j))->G = (int)sum_g;
            sum_b = MAX(sum_b,255), sum_b = MIN(sum_b,0);
            (image_out->img_data + (img_i*img_col+img_j))->B = (int)sum_b;
        }
    }

    return image_out;
    
}

void writePPM(const char *file, PPMImage *image)
{
    FILE *fp;

    fp = fopen(file,"wb");
    if (!fp) 
    {
        printf("Unable to open file %s\n", file);
        exit(0);
    }

    fprintf(fp, "P3\n");
    fprintf(fp,"%d %d\n", image->len, image->wid);
    fprintf(fp, "%d\n", RGB_COMP);

    int i = 0, lineLen = 1;
    while (i < image->len * image->wid) {
        /* NOTE: The 35 arbitrary, but is made to conform with the line width standards */
        if ((35 % lineLen) == 35) {
            fprintf(fp, "\n");
            lineLen = 1;
        }
        /* Get each pixel and write out its components in their ascii representation */
        fprintf(fp, "%d ", (image->img_data +i)->R);
        fprintf(fp, "%d ", (image->img_data +i)->G);
        fprintf(fp, "%d ", (image->img_data +i)->B);
        lineLen++;
        i++;
        }


    fclose(fp);
    
}
