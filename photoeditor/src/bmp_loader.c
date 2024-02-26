#include <stdio.h>
#include "../inc/bmp_loader.h"
#include "stdlib.h"

void free_bitmap(bitmap_t *bitmap)
{
    if (bitmap == NULL)
        return;
    if (bitmap->imageData.red != NULL)
        free(bitmap->imageData.red);
    if (bitmap->imageData.blue != NULL)
        free(bitmap->imageData.blue);
    if (bitmap->imageData.green != NULL)
        free(bitmap->imageData.green);
    free(bitmap);
}

bitmap_t *get_out_bitmap(bitmap_t *bitmap)
{
    bitmap_t *b = malloc(sizeof(bitmap_t));
    if (b == NULL)
        return NULL;
    *b = *bitmap;
    b->imageData.blue = malloc(sizeof(BYTE) * bitmap->bitmapInfoHeader.biWidth * bitmap->bitmapInfoHeader.biHeight);
    b->imageData.green = malloc(sizeof(BYTE) * bitmap->bitmapInfoHeader.biWidth * bitmap->bitmapInfoHeader.biHeight);
    b->imageData.red = malloc(sizeof(BYTE) * bitmap->bitmapInfoHeader.biWidth * bitmap->bitmapInfoHeader.biHeight);

    if (b->imageData.blue == NULL || b->imageData.green == NULL || b->imageData.red == NULL)
    {
        free_bitmap(b);
        b = NULL;
    }

    return b;
}

int read_headers(BITMAPFILEHEADER *bitmapFileHeader, BITMAPINFOHEADER *bitmapInfoHeader, FILE *file)
{
    int rc = EXIT_SUCCESS;
    if (fread(bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, file) != 1)
        rc = ERR_READ_FILE;

    if (bitmapFileHeader->bfType != 0x4D42)
        rc = ERR_FORMAT_FILE;

    if (fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, file) != 1)
        rc = ERR_READ_FILE;

    return rc;
}

int alloc_channels(BYTE **blue, BYTE **green, BYTE **red, size_t size)
{
    int rc = EXIT_SUCCESS;

    *blue = (BYTE *) malloc(size);
    *green = (BYTE *) malloc(size);
    *red = (BYTE *) malloc(size);

    if (*blue == NULL || *red == NULL || *green == NULL)
    {
        rc = ERR_ALLOC_MEM;
        if (*blue != NULL)
            free(*blue);
        if (*green != NULL)
            free(*green);
        if (*red != NULL)
            free(*red);
    }

    return rc;
}

int read_channels(BYTE *blue, BYTE *green, BYTE *red, LONG width, LONG height, FILE *file)
{
    int rc = EXIT_SUCCESS;
    char buff[4];
    for (size_t i = 0; (i < height) && !rc; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (fread(blue + i * width + j, sizeof(BYTE), 1, file) != 1)
                rc = ERR_READ_FILE;
            if (fread(green + i * width + j, sizeof(BYTE), 1, file) != 1)
                rc = ERR_READ_FILE;
            if (fread(red + i * width + j, sizeof(BYTE), 1, file) != 1)
                rc = ERR_READ_FILE;
        }
         if (width % 4 != 0)
            if (fread(buff, sizeof (BYTE) * (width % 4), 1, file) != 1)
                rc = ERR_READ_FILE;
    }

    return rc;
}

int load_bmp(char *filename, bitmap_t *bitmap)
{
    FILE *file;
    BITMAPFILEHEADER bitmapFileHeader;
    BITMAPINFOHEADER bitmapInfoHeader;

    file = fopen(filename, "rb");
    if (file == NULL)
        return ERR_OPEN_FILE;

    int rc = EXIT_SUCCESS;

    rc = read_headers(&bitmapFileHeader, &bitmapInfoHeader, file);

    BYTE *blue, *green, *red;

    !rc && fseek(file, bitmapFileHeader.bfOffBits, SEEK_SET);
    !rc && (rc = alloc_channels(&blue, &green, &red, bitmapInfoHeader.biHeight * bitmapInfoHeader.biWidth));

    if (!rc && ((rc = read_channels(blue, green, red, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, file)) != EXIT_SUCCESS))
    {
        free(blue);
        free(green);
        free(red);
        blue = NULL;
        green = NULL;
        red = NULL;
    }

    fclose(file);

    bitmap->bitmapFileHeader = bitmapFileHeader;
    bitmap->bitmapInfoHeader = bitmapInfoHeader;
    bitmap->imageData.blue = blue;
    bitmap->imageData.green = green;
    bitmap->imageData.red = red;

    return rc;
}

int write_channels(bitmap_t *bitmap, FILE *file)
{
    int rc = EXIT_SUCCESS;
    int h = bitmap->bitmapInfoHeader.biHeight;
    int w = bitmap->bitmapInfoHeader.biWidth;
    char buff[4] = {0};
    for (size_t i = 0; (i < h) && !rc; i++)
    {
        for (size_t j = 0; (j < w) && !rc; j++)
        {
            if (fwrite(bitmap->imageData.blue + i * w + j, sizeof(BYTE), 1, file) != 1)
                rc = ERR_WRITE_FILE;
            if (fwrite(bitmap->imageData.green + i * w + j, sizeof(BYTE), 1, file) != 1)
                rc = ERR_WRITE_FILE;
            if (fwrite(bitmap->imageData.red + i * w + j, sizeof(BYTE), 1, file) != 1)
                rc = ERR_WRITE_FILE;
        }
        if (w % 4 != 0)
            if (fwrite(buff, sizeof(BYTE) * (w % 4), 1, file) != 1)
                rc = ERR_WRITE_FILE;
    }

    return rc;
}

int save_bmp(char *filename, bitmap_t *bitmap)
{
    FILE *file;
    file = fopen(filename, "wb");
    int rc = EXIT_SUCCESS;

    if (file == NULL)
        return ERR_OPEN_FILE;

    if (!rc && (fwrite(&bitmap->bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, file) != 1))
        rc = ERR_WRITE_FILE;
    if (!rc && (fwrite(&bitmap->bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, file) != 1))
        rc = ERR_WRITE_FILE;
    if (!rc && ((rc = write_channels(bitmap, file)) != EXIT_SUCCESS))
        rc = ERR_WRITE_FILE;
    fclose(file);

    return rc;
}

