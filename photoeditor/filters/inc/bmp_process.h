#ifndef EDITOR_BMP_PROCESS_H
#define EDITOR_BMP_PROCESS_H

#include "../../inc/types.h"

void apply_func_bitmap(bitmap_t *bitmap, BYTE(*func)(BYTE));

void normalize(bitmap_t *bitmap);

void merge_bitmaps(bitmap_t *src1, bitmap_t *src2, bitmap_t *res, BYTE(*merge_method)(BYTE, BYTE));

#endif //EDITOR_BMP_PROCESS_H
