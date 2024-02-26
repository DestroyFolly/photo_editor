#include <limits.h>
#include "../inc/bmp_process.h"

void apply_func_bitmap(bitmap_t *bitmap, BYTE(*func)(BYTE))
{
    for (int i = 0; i < bitmap->bitmapInfoHeader.biHeight; i++)
    {
        for (int j = 0; j < bitmap->bitmapInfoHeader.biWidth; j++)
        {
            int ind = i * bitmap->bitmapInfoHeader.biWidth + j;
            bitmap->imageData.blue[ind] = func(bitmap->imageData.blue[ind]);
            bitmap->imageData.green[ind] = func(bitmap->imageData.green[ind]);
            bitmap->imageData.red[ind] = func(bitmap->imageData.red[ind]);
        }
    }
}

void merge_bitmaps(bitmap_t *src1, bitmap_t *src2, bitmap_t *res, BYTE(*merge_method)(BYTE, BYTE))
{
    for (int i = 0; i < src1->bitmapInfoHeader.biHeight; i++)
    {
        for (int j = 0; j < src1->bitmapInfoHeader.biWidth; j++)
        {
            int ind = i * src1->bitmapInfoHeader.biWidth + j;
            res->imageData.blue[ind] = merge_method(src1->imageData.blue[ind], src2->imageData.blue[ind]);
            res->imageData.green[ind] = merge_method(src1->imageData.green[ind], src2->imageData.green[ind]);
            res->imageData.red[ind] = merge_method(src1->imageData.red[ind], src2->imageData.red[ind]);
        }
    }
}

void find_min_max(bitmap_t *bitmap, BYTE *channel, BYTE *min, BYTE *max)
{
    *min = UCHAR_MAX;
    *max = 0;
    for (int i = 0; i < bitmap->bitmapInfoHeader.biHeight; i++)
    {
        for (int j = 0; j < bitmap->bitmapInfoHeader.biWidth; j++)
        {
            if (channel[i * bitmap->bitmapInfoHeader.biWidth + j] < *min)
                *min = channel[i * bitmap->bitmapInfoHeader.biWidth + j];

            if (channel[i * bitmap->bitmapInfoHeader.biWidth + j] > *max)
                *max = channel[i * bitmap->bitmapInfoHeader.biWidth + j];
        }
    }
}

void normalize(bitmap_t *bitmap)
{
    BYTE min, max;
    find_min_max(bitmap, bitmap->imageData.blue, &min, &max);
    float k = UCHAR_MAX;
    if (max != min)
        k /= (float) (max - min);
    for (int i = 0; i < bitmap->bitmapInfoHeader.biHeight; i++)
    {
        for (int j = 0; j < bitmap->bitmapInfoHeader.biWidth; j++)
        {
            int ind = i * bitmap->bitmapInfoHeader.biWidth + j;
            bitmap->imageData.blue[ind] = (bitmap->imageData.blue[ind] - min) * k;
            bitmap->imageData.green[ind] = (bitmap->imageData.green[ind] - min) * k;
            bitmap->imageData.red[ind] = (bitmap->imageData.red[ind] - min) * k;
        }
    }
}