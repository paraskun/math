#ifndef SMTX

#include <math.h>
#include <stdlib.h>

#include <mtx.h>
#include <sle.h>

static inline void swap(int* p, int a, int b) {
  if (a == b)
    return;

  real tmp = p[a];
  p[a] = p[b];
  p[b] = tmp;
}

void sle_gauss(struct mtx* mp, struct vec* yp, struct vec* bp) {
  int n = mp->n;
  int* pp = malloc(sizeof(int) * n);

  real* mvp = mp->v;
  real* yvp = yp->v;
  real* bvp = bp->v;

  for (int i = 0; i < n; ++i)
    pp[i] = i;

  for (int i = 0; i < n; ++i) {
    real max = fabs(mvp[pp[i] * n + i]);
    int maxi = i;

    for (int j = i + 1, rj = pp[i + 1] * n; j < n; ++j, rj += n) {
      real mij = fabs(mvp[rj + i]);

      if (mij > max) {
        max = mij;
        maxi = j;
      }
    }

    swap(pp, i, maxi);

    int ri = pp[i] * n;

    for (int j = i + 1, rj = pp[i + 1] * n; j < n; ++j, rj += n) {
      real k = mvp[rj + i] / mvp[ri + i];

      for (int c = i + 1; c < n; ++c)
        mvp[rj + c] -= mvp[ri + c] * k;

      bvp[pp[j]] -= bvp[pp[i]] * k;
    }
  }

  for (int o = 0, i; o < n; ++o) {
    i = n - 1 - o;

    yvp[i] = bvp[pp[i]];

    for (int j = i + 1; j < n; ++j)
      yvp[i] -= yvp[j] * mvp[pp[i] * n + j];

    yvp[i] /= mvp[pp[i] * n + i];
  }

  free(pp);
}

#endif  // DMTX
