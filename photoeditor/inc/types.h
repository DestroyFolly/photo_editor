#ifndef EDITOR_TYPES_H
#define EDITOR_TYPES_H

#include <stdint.h>

typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef int32_t LONG;
typedef unsigned char BYTE;

#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;
    DWORD bfSize;
    DWORD bfReserved;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

#pragma pack(pop)

#pragma pack(push, 1)
typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)


typedef struct imageData
{
    BYTE *blue;
    BYTE *green;
    BYTE *red;
} imageData_t;

typedef struct bitmapData
{
    BITMAPFILEHEADER bitmapFileHeader;
    BITMAPINFOHEADER bitmapInfoHeader;
    imageData_t imageData;
} bitmap_t;

#endif //EDITOR_TYPES_H
