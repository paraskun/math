#include "../mtx/mtx_dense.c"
#include "sle.h"

#include <stdint.h>

void sle_gauss_revu(struct mtx *a, struct vec *x, struct vec *b) {
  for (int32_t i = a->n - 1; i >= 0; --i) {
    x->v[i] = b->v[i] / a->v[i * a->n + i];

    for (int32_t j = i + 1; j < a->n; ++j)
      x->v[i] -= (x->v[j] * a->v[i * a->n + j]) / a->v[i * a->n + i];
  }
}
