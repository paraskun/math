#include <mtx_dns.h>
#include <sle.h>

#ifndef TRDS
#define TRDS 1
#endif  // TRDS

void sle_gauss_u(struct mtx* a, struct vec* x, struct vec* b) {
  for (int32_t i = a->n - 1; i >= 0; --i) {
    real xi = b->v[i] / a->v[i * a->n + i];

#pragma omp parallel for reduction(- : xi) num_threads(TRDS)
    for (int32_t j = i + 1; j < a->n; ++j)
      xi -= (x->v[j] * a->v[i * a->n + j]) / a->v[i * a->n + i];

    x->v[i] = xi;
  }
}
