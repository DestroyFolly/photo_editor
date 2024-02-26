#include "../inc/sobel.h"
#include "math.h"
#include "../inc/bmp_process.h"
#include "../../inc/bmp_loader.h"
#include "../inc/filters_errors.h"
#include "stdlib.h"

kernel_t *get_sobel_x_kernel()
{
    kernel_t *kernel = get_zero_kernel(3, 3);
    if (kernel == NULL)
        return NULL;

    kernel->data[0][0] = -1;
    kernel->data[1][0] = -2;
    kernel->data[2][0] = -1;

    kernel->data[0][2] = 1;
    kernel->data[1][2] = 2;
    kernel->data[2][2] = 1;

    return kernel;
}

kernel_t *get_sobel_y_kernel()
{
    kernel_t *kernel = get_zero_kernel(3, 3);
    if (kernel == NULL)
        return NULL;

    kernel->data[0][0] = -1;
    kernel->data[0][1] = -2;
    kernel->data[0][2] = -1;

    kernel->data[2][0] = 1;
    kernel->data[2][1] = 2;
    kernel->data[2][2] = 1;

    return kernel;
}

BYTE quad_avg(BYTE c1, BYTE c2)
{
    return sqrt(c1 * c1 + c2 * c2);
}

int sobel(bitmap_t *bitmap, bitmap_t *res)
{
    int rc = EXIT_SUCCESS;
    bitmap_t *bx = get_out_bitmap(bitmap);
    bitmap_t *by = get_out_bitmap(bitmap);
    if (bx == NULL || by == NULL)
        rc = ERR_ALLOC_BITMAP;
    else
    {
        kernel_t *kx = get_sobel_x_kernel();
        kernel_t *ky = get_sobel_y_kernel();
        if (kx == NULL || ky == NULL)
            rc = ERR_ALLOC_KERNEL;
        else
        {
            convolution(bitmap, bx, kx);
            convolution(bitmap, by, ky);
            merge_bitmaps(bx, by, res, quad_avg);
            free_kernel(kx);
            free_kernel(ky);
        }
        free_bitmap(bx);
        free_bitmap(by);
    }
    return rc;
}