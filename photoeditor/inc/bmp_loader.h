#ifndef EDITOR_BMP_LOADER_H
#define EDITOR_BMP_LOADER_H
#include "types.h"

#define ERR_OPEN_FILE 3
#define ERR_READ_FILE 4
#define ERR_FORMAT_FILE 5
#define ERR_ALLOC_MEM 6
#define ERR_WRITE_FILE 7

int load_bmp(char *filename, bitmap_t *bitmap);
int save_bmp(char *filename, bitmap_t *bitmap);
bitmap_t *get_out_bitmap(bitmap_t *bitmap);
void free_bitmap(bitmap_t *bitmap);

#endif //EDITOR_BMP_LOADER_H
