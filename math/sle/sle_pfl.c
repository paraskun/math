#include <def.h>

#ifdef MTX_PFL
#ifndef SLE_PFL_H
#define SLE_PFL_H

#include <string.h>

#include <mtx_pfl.h>
#include <sle.h>

static void sle_gauss_l(struct mtx* a, struct vec* x, struct vec* b) {
  for (int i = 0; i < x->n; ++i) {
    int c = a->p[i + 1] - a->p[i];
    double e = b->v[i];

    for (int k = 1; k <= c; ++k)
      e -= a->l[a->p[i + 1] - k] * x->v[i - k];

    x->v[i] = e;
  }
}

static void sle_gauss_d(struct mtx* a, struct vec* x, struct vec* b) {
  for (int i = 0; i < x->n; ++i) {
    double e = b->v[i] / a->d[i];
    x->v[i] = e;
  }
}

static void sle_gauss_u(struct mtx* a, struct vec* x, struct vec* b) {
  memcpy(x->v, b->v, sizeof(real) * x->n);

  for (int i = x->n - 1; i > 0; --i) {
    int c = a->p[i + 1] - a->p[i];

    for (int k = 0; k < c; ++k)
      x->v[i - k - 1] -= a->u[a->p[i] + c - 1 - k] * x->v[i];
  }
}

void sle_gauss(struct mtx* a, struct vec* x, struct vec* b) {
  mtx_ldu(a);

  sle_gauss_l(a, x, b);
  sle_gauss_d(a, b, x);
  sle_gauss_u(a, x, b);
}

#endif  // SLE_PFL_H
#endif  // MTX_PFL
