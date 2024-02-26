#ifndef EDITOR_LAPLACE_H
#define EDITOR_LAPLACE_H
#include "kernel.h"

int laplace_pos(bitmap_t *src, bitmap_t *res);
int laplace_neg(bitmap_t *src, bitmap_t *res);
int laplace_diag(bitmap_t *src, bitmap_t *res);

#endif //EDITOR_LAPLACE_H
