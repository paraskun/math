#include <mtx_pfl.h>
#include <sle_pfl.h>

#include <string.h>

static void ssle_l(struct mtx_pfl* a, struct vec* y, struct vec* b) {
  int n = y->n;
  int* pp = a->p;

  real* lp = a->l;
  real* yp = y->v;
  real* bp = b->v;

  for (int i = 0; i < n; ++i) {
    synt sum = 0;

    int k0 = pp[i];
    int k1 = pp[i + 1];
    int j0 = i - k1 + k0;

    for (int k = k0, j = j0; k < k1; ++k, ++j)
      sum += lp[k] * yp[j];

    yp[i] = bp[i] - sum;
  }
}

static void ssle_d(struct mtx_pfl* a, struct vec* y, struct vec* b) {
  int n = y->n;

  real* dp = a->d;
  real* yp = y->v;
  real* bp = b->v;

  for (int i = 0; i < n; ++i) {
    synt e = bp[i] / dp[i];
    yp[i] = e;
  }
}

static void ssle_u(struct mtx_pfl* a, struct vec* y, struct vec* b) {
  int n = y->n;
  int* pp = a->p;

  real* up = a->u;
  real* yp = y->v;
  real* bp = b->v;

  memcpy(yp, bp, sizeof(real) * n);

  for (int j = n - 1; j > 0; --j) {
    int k0 = pp[j];
    int k1 = pp[j + 1];
    int i0 = j - k1 + k0;

    for (int k = k0, i = i0; k < k1; ++k, ++i)
      yp[i] -= up[k] * yp[j];
  }
}

void ssle_ldu(struct mtx_pfl* a, struct vec* x, struct vec* b) {
  mtx_pfl_ldu(a);

  ssle_l(a, x, b);
  ssle_d(a, b, x);
  ssle_u(a, x, b);
}
