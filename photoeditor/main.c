#include <string.h>
#include <stdlib.h>
#include "stdio.h"
#include "inc/bmp_loader.h"
#include "inc/filters.h"

// консоль
// infile outfile
// g Перевод изображения в градацию серого
// s Оператор Собеля (градиент изображения)
// l Оператор Лапласа (вторая производная изображения)
// b Размытие по Гауссу
// e Выделение границ (опционально, используя оператор Канни)
// k Обработка произвольно заданным ядром

#define ERR_ARGS 10
#define ERR_ALLOC_BMP 11

int own_kernel(bitmap_t *bitmap, bitmap_t *res)
{
    int rc = EXIT_SUCCESS;

    int kernel_size;
    printf("kernel size: ");
    if (scanf("%d", &kernel_size) != 1)
        rc = ERR_ARGS;
    if (kernel_size < 1 || (kernel_size % 2 != 1))
        rc = ERR_ARGS;
    kernel_t *kernel = get_zero_kernel(kernel_size, kernel_size);
    if (kernel == NULL)
        rc = ERR_ALLOC_MEM;

    for (int i = 0; i < kernel_size; i++)
    {
        for (int j = 0; j < kernel_size; j++)
        {
            if (scanf("%f", &kernel->data[i][j])!= 1)
                rc = ERR_ARGS;
        }
    }
    if (rc == EXIT_SUCCESS)
        convolution(bitmap, res, kernel); // проверить на нулевую сумму

    return rc;
}


int main(int argc, char **argv)
{
    if(argc != 4)
        return ERR_ARGS;

    int rc = EXIT_SUCCESS;
    bitmap_t src;

    rc = load_bmp(argv[1], &src);
    if (rc)
    {
        free(src.imageData.blue);
        free(src.imageData.red);
        free(src.imageData.green);
    }
    bitmap_t *res =  get_out_bitmap(&src);
    if (res == NULL)
        rc = ERR_ALLOC_BMP;

    if(strcmp(argv[3], "g") == 0 && !rc)
        classic_grayscale(&src, res);
    else if(strcmp(argv[3], "s") == 0)
        rc = rc || sobel(&src, res);
    else if(strcmp(argv[3], "l") == 0)
        rc = rc || laplace_pos(&src, res);
    else if(strcmp(argv[3], "b") == 0)
        rc = rc || gaussian_blur(&src, res, 30, 30);
    else if(strcmp(argv[3], "e") == 0)
        rc = rc || edge_detection(&src, res);
    else if(strcmp(argv[3], "k") == 0)
        rc = rc || own_kernel(&src, res);
    else
        rc = ERR_ARGS;

    rc = rc || save_bmp(argv[2], res);

    if(!rc)
    {
        free_bitmap(res);
        free(src.imageData.blue);
        free(src.imageData.red);
        free(src.imageData.green);
    }

    return rc;
}

