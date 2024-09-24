#include <def.h>

#ifdef DMTX

#include <mtx/dmtx.h>
#include <sle/sle.h>

void sle_gauss(struct mtx* a, struct vec* x, struct vec* b) {
  for (int i = a->n - 1; i >= 0; --i) {
    real xi = b->v[i] / a->v[i * a->n + i];

pragma omp parallel for reduction(- : xi) num_threads(TN)
    for (size_t j = i + 1; j < a->n; ++j)
      xi -= (x->v[j] * a->v[i * a->n + j]) / a->v[i * a->n + i];

    x->v[i] = xi;
  }
}

#endif  // MTX_DNS
