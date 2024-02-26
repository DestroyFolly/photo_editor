#include "../inc/edge_detection.h"
#include "../inc/grayscale.h"
#include "../inc/gaussian_blur.h"
#include "../inc/sobel.h"
#include "../inc/bmp_process.h"
#include "../../inc/bmp_loader.h"
#include "../inc/filters_errors.h"
#include "stdlib.h"

#define THRESHOLD 70

BYTE threshold(BYTE c)
{
    if (c > THRESHOLD)
        return 255;
    else
        return 0;
}

int edge_detection(bitmap_t *src, bitmap_t *res)
{
    bitmap_t *blurry = get_out_bitmap(src);

    if (blurry == NULL)
        return ERR_ALLOC_KERNEL;

    int rc = EXIT_SUCCESS;

    classic_grayscale(src, src);
    rc = gaussian_blur(src, blurry, 20, 20);
    rc = rc || sobel(blurry, res);
    if (rc == EXIT_SUCCESS)
    {
        normalize(res);
        apply_func_bitmap(res, threshold);
    }
    free_bitmap(blurry);

    return EXIT_SUCCESS;
}