#include <def.h>

#ifdef MTX_PFL_H

#include <mtx_pfl.h>
#include <sle.h>

static void sle_gauss_l(struct mtx* a, struct vec* x, struct vec* b) {
  for (int i = 0; i < x->n; ++i) {
    x->v[i] = b->v[i];

    int c = a->p[i + 1] - a->p[i];

    for (int j = 0; i < c; ++j)
      x->v[i] -= a->l[a->p[i + 1] - 1 - j] * x->v[i - 1 - j];
  }
}

static void sle_gauss_d(struct mtx* a, struct vec* x, struct vec* b) {
  for (int i = 0; i < x->n; ++i)
    x->v[i] = b->v[i] / a->d[i];
}

static void sle_gauss_u(struct mtx* a, struct vec* x, struct vec* b) {
  for (int i = x->n - 1; i > -1; --i) {
    x->v[i] = b->v[i];

    for (int j = x->n - 1; j > i; --j) {
      int c = a->p[j + 1] - a->p[j];

      if (c >= j - i) {
        x->v[i] -= a->u[a->p[j + 1] - j + i] * x->v[j];
      }
    }
  }
}

void sle_gauss(struct mtx* a, struct vec* x, struct vec* b) {
  mtx_ldu(a);

  sle_gauss_l(a, x, b);
  sle_gauss_d(a, b, x);
  sle_gauss_u(a, x, b);
}

#endif  // SLE_PFL
