#include "../inc/laplace.h"
#include "../inc/bmp_process.h"
#include "../inc/filters_errors.h"
#include "stdlib.h"

kernel_t *get_laplace_pos_kernel()
{
    kernel_t *kernel = get_zero_kernel(3, 3);
    if (kernel == NULL)
        return NULL;

    kernel->data[1][0] = 1;
    kernel->data[1][1] = -4;
    kernel->data[1][2] = 1;

    kernel->data[0][1] = 1;
    kernel->data[2][1] = 1;

    return kernel;
}

kernel_t *get_laplace_neg_kernel()
{
    kernel_t *kernel = get_zero_kernel(3, 3);
    if (kernel == NULL)
        return NULL;

    kernel->data[1][0] = -1;
    kernel->data[1][1] = 4;
    kernel->data[1][2] = -1;

    kernel->data[0][1] = -1;
    kernel->data[2][1] = -1;

    return kernel;
}

kernel_t *get_laplace_diag_kernel()
{
    kernel_t *kernel = get_zero_kernel(3, 3);
    if (kernel == NULL)
        return NULL;

    kernel->data[0][0] = 1;
    kernel->data[0][1] = 1;
    kernel->data[0][2] = 1;

    kernel->data[1][0] = 1;
    kernel->data[1][1] = -8;
    kernel->data[1][2] = 1;

    kernel->data[2][0] = 1;
    kernel->data[2][1] = 1;
    kernel->data[2][2] = 1;

    return kernel;
}

int normalize_laplace(bitmap_t *src, bitmap_t *res, kernel_t *(get_kernel)())
{
    kernel_t *kernel = get_kernel();
    if(kernel == NULL)
        return ERR_ALLOC_KERNEL;

    convolution(src, res, kernel);
    normalize(res);

    free_kernel(kernel);

    return EXIT_SUCCESS;
}

int laplace_pos(bitmap_t *src, bitmap_t *res)
{
    return normalize_laplace(src, res, get_laplace_pos_kernel);
}

int laplace_neg(bitmap_t *src, bitmap_t *res)
{
    return normalize_laplace(src, res, get_laplace_neg_kernel);
}

int laplace_diag(bitmap_t *src, bitmap_t *res)
{
    return normalize_laplace(src, res, get_laplace_diag_kernel);
}

