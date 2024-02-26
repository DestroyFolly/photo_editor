#include "malloc.h"
#include "../inc/kernel.h"
#include "math.h"

void free_kernel(kernel_t *kernel)
{
    for (int i = 0; i < kernel->h; i++)
        if (kernel->data[i] != NULL)
            free(kernel->data[i]);

    free(kernel->data);
    free(kernel);
}

kernel_t *alloc_kernel(int h, int w)
{
    kernel_t *res = malloc(sizeof(kernel_t));
    if(res != NULL)
    {
        res->data = calloc(h, sizeof(int *));
        res->h = h;
        res->w = w;
        if (res->data == NULL)
        {
            free(res);
            res = NULL;
        }
        else
        {
            for (int i = 0; i < h; i++)
            {
                res->data[i] = calloc(h, sizeof(int));
                if (res->data[i] == NULL)
                {
                    free_kernel(res);
                    return NULL;
                }
            }
        }
    }

    return res;
}

kernel_t *get_zero_kernel(int h, int w)
{
    if (h <= 0 || w <= 0)
        return NULL;

    kernel_t *kernel = alloc_kernel(h, w);

    return kernel;
}

float calc_kernel(bitmap_t *bitmap, kernel_t *kernel, int posX, int posY, BYTE *channel)
{
    float sum = 0;
    float maskSum = 0;

    for (int i = 0, ind_x = posX - kernel->h / 2; i < kernel->h; i++, ind_x++)
    {
        int pixel_pos_x = ind_x;
        for (int j = 0, ind_y = posY - kernel->w / 2; j < kernel->w; j++, ind_y++)
        {
            int pixel_pos_y = ind_y;

            if (ind_x < 0)
                pixel_pos_x = 0;
            if (ind_y < 0)
                pixel_pos_y = 0;
            if (ind_x > bitmap->bitmapInfoHeader.biHeight - 1)
                pixel_pos_x = bitmap->bitmapInfoHeader.biHeight - 1;
            if (ind_y > bitmap->bitmapInfoHeader.biWidth - 1)
                pixel_pos_y = bitmap->bitmapInfoHeader.biWidth - 1;

            sum += kernel->data[i][j] * channel[pixel_pos_x * bitmap->bitmapInfoHeader.biWidth + pixel_pos_y];
            maskSum += fabs(kernel->data[i][j]);
        }
    }

    return  fabs(sum / maskSum);
}


void convolution(bitmap_t *src, bitmap_t *res, kernel_t *mask)
{
    for (int i = 0; i < src->bitmapInfoHeader.biHeight; i++)
    {
        for (int j = 0; j < src->bitmapInfoHeader.biWidth; j++)
        {
            res->imageData.blue[i * src->bitmapInfoHeader.biWidth + j] = calc_kernel(src, mask, i, j, src->imageData.blue);
            res->imageData.green[i * src->bitmapInfoHeader.biWidth + j] = calc_kernel(src, mask, i, j, src->imageData.green);
            res->imageData.red[i * src->bitmapInfoHeader.biWidth + j] = calc_kernel(src, mask, i, j, src->imageData.red);
        }
    }
}