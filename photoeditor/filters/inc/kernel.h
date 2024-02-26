#ifndef EDITOR_KERNEL_H
#define EDITOR_KERNEL_H
#include "../../inc/types.h"
typedef struct kernel
{
    float **data;
    int h;
    int w;
} kernel_t;

kernel_t *get_zero_kernel(int h, int w);

void convolution(bitmap_t *src, bitmap_t *res, kernel_t *mask);

void free_kernel(kernel_t *kernel);

#endif //EDITOR_KERNEL_H
