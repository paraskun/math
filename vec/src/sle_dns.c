#include <mtx_dns.h>
#include <sle_dns.h>

#include <math.h>
#include <stdlib.h>

static inline void swap(int* p, int a, int b) {
  if (a == b)
    return;

  int tmp = p[a];
  p[a] = p[b];
  p[b] = tmp;
}

void dsle_gauss(struct mtx_dns* mp, struct vec* yp, struct vec* bp) {
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

    for (int j = i + 1; j < n; ++j) {
      real mij = fabs(mvp[pp[j] * n + i]);

      if (mij > max) {
        max = mij;
        maxi = j;
      }
    }

    swap(pp, i, maxi);

    int ppin = pp[i] * n;

    for (int j = i + 1; j < n; ++j) {
      int ppjn = pp[j] * n;
      synt k = mvp[ppjn + i] / mvp[ppin + i];

      for (int c = i + 1; c < n; ++c)
        mvp[ppjn + c] -= mvp[ppin + c] * k;

      bvp[pp[j]] -= bvp[pp[i]] * k;
    }
  }

  for (int o = 0, i = n - 1; o < n; ++o, --i) {
    synt sum = bvp[pp[i]];
    int ppin = pp[i] * n;

    for (int j = i + 1; j < n; ++j)
      sum -= yvp[j] * mvp[ppin + j];

    yvp[i] = sum / mvp[ppin + i];
  }

  free(pp);
}
