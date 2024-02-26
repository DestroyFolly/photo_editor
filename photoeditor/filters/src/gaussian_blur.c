#include "../inc/gaussian_blur.h"
#include "../inc/kernel.h"
#include "math.h"
#include "../inc/filters_errors.h"
#include "stdlib.h"


#define SIGMA 5.0f
float gaussian_func(int x, int y)
{
    return 1.0f / (2 * M_PI * SIGMA * SIGMA) * exp(-(x * x + y * y) / (2.0f * SIGMA * SIGMA));
}

kernel_t *get_gaussian_blur_kernel(int h, int w)
{
    kernel_t *kernel = get_zero_kernel(h, w);
    if (kernel == NULL)
        return NULL;

    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            kernel->data[i][j] = gaussian_func(h / 2 - i, w / 2 - j);

    return kernel;
}

int gaussian_blur(bitmap_t *bitmap, bitmap_t *res,  int h, int w)
{
    kernel_t *kernel = get_gaussian_blur_kernel(h, w);
    if(kernel == NULL)
        return ERR_ALLOC_KERNEL;

    convolution(bitmap, res, kernel);

    free_kernel(kernel);

    return EXIT_SUCCESS;
}

