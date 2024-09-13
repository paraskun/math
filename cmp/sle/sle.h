#ifndef SLE_H
#define SLE_H

#include "../mtx/mtx.h"
#include "../vec/vec.h"

void sle_gauss_u(struct mtx* a, struct vec* x, struct vec* b);

#endif  // SLE_H
