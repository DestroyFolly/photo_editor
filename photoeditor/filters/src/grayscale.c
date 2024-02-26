#include "../inc/grayscale.h"

BYTE calc_classic_grayscale_color(BYTE b, BYTE g, BYTE r)
{
    return (b + g + r) / 3;
}

#define KR 0.299f
#define KB 0.114f

BYTE calc_brightness_grayscale_color(BYTE b, BYTE g, BYTE r)
{
    return KR * r + (1 - KR - KB) * g + KB * b;
}

void grayscale(bitmap_t *src, bitmap_t *res, BYTE(*grayscale_method)(BYTE, BYTE, BYTE))
{
    for (int i = 0; i < src->bitmapInfoHeader.biHeight * src->bitmapInfoHeader.biWidth; i++)
    {
        BYTE b = src->imageData.blue[i];
        BYTE g = src->imageData.green[i];
        BYTE r = src->imageData.red[i];
        BYTE color = grayscale_method(b, g, r);
        res->imageData.blue[i] = color;
        res->imageData.green[i] = color;
        res->imageData.red[i] = color;
    }
}

void classic_grayscale(bitmap_t *src, bitmap_t *res)
{
    grayscale(src, res, calc_classic_grayscale_color);
}

void brightness_grayscale(bitmap_t *src, bitmap_t *res)
{
    grayscale(src, res, calc_brightness_grayscale_color);
}
