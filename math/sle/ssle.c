#include <def.h>

#ifdef SMTX

#include <string.h>

#include <mtx/smtx.h>
#include <sle/sle.h>

static void sle_l(struct mtx* a, struct vec* x, struct vec* b) {
  for (size_t i = 0; i < x->n; ++i) {
    size_t c = a->p[i + 1] - a->p[i];
    preal e = b->v[i];

    for (size_t k = 1; k <= c; ++k)
      e -= a->l[a->p[i + 1] - k] * x->v[i - k];

    x->v[i] = e;
  }
}

static void sle_d(struct mtx* a, struct vec* x, struct vec* b) {
  for (size_t i = 0; i < x->n; ++i) {
    preal e = b->v[i] / a->d[i];
    x->v[i] = e;
  }
}

static void sle_u(struct mtx* a, struct vec* x, struct vec* b) {
  memcpy(x->v, b->v, sizeof(real) * x->n);

  for (size_t i = x->n - 1; i > 0; --i) {
    size_t c = a->p[i + 1] - a->p[i];

    for (size_t k = 0; k < c; ++k)
      x->v[i - k - 1] -= a->u[a->p[i] + c - 1 - k] * x->v[i];
  }
}

void sle_ldu(struct mtx* a, struct vec* x, struct vec* b) {
  mtx_ldu(a);

  sle_l(a, x, b);
  sle_d(a, b, x);
  sle_u(a, x, b);
}

#endif  // MTX_PFL
