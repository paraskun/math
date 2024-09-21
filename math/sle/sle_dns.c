#include <def.h>

#ifdef MTX_DNS
#ifndef SLE_DNS_H
#define SLE_DNS_H

#include <mtx/mtx_dns.h>
#include <sle/sle.h>

void sle_gauss(struct mtx* a, struct vec* x, struct vec* b) {
  for (int i = a->n - 1; i >= 0; --i) {
    real xi = b->v[i] / a->v[i * a->n + i];

#pragma omp parallel for reduction(- : xi) num_threads(THRD)
    for (size_t j = i + 1; j < a->n; ++j)
      xi -= (x->v[j] * a->v[i * a->n + j]) / a->v[i * a->n + i];

    x->v[i] = xi;
  }
}

#endif  // SLE_DNS_H
#endif  // MTX_DNS
