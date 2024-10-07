#include <dmtx.h>
#include <dsle.h>

#include <math.h>
#include <stdlib.h>

static inline void swap(int* p, int a, int b) {
  if (a == b)
    return;

  int tmp = p[a];
  p[a] = p[b];
  p[b] = tmp;
}

void dsle_gauss(struct dmtx* mp, struct vec* yp, struct vec* bp) {
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

    printf("max: %d\n", maxi);

    swap(pp, i, maxi);

    for (int j = 0; j < mp->n; ++j)
      printf("%.2lf ", mvp[pp[i] * n + j]);

    putchar('\n');

    for (int j = i + 1; j < n; ++j) {
      real k = mvp[pp[j] * n + i] / mvp[pp[i] * n + i];

      printf("%lf\n", k);

      for (int c = 0; c <= i; ++c)
        mvp[pp[j] * n + c] = 0;

      for (int c = i + 1; c < n; ++c)
        mvp[pp[j] * n + c] -= mvp[pp[i] * n + c] * k;

      bvp[pp[j]] -= bvp[pp[i]] * k;

      for (int k = 0; k < mp->n; ++k)
        printf("%.2lf ", mvp[pp[j] * n + k]);

      putchar('\n');
    }
  }

  FILE* f = fopen("test", "w+");

  dmtx_fput(f, mp);

  for (int i = 0; i < mp->n; ++i)
    fprintf(f, "%d ", pp[i]);

  fputc('\n', f);

  for (int i = 0; i < mp->n; ++i)
    fprintf(f, "%.2lf ", bvp[i]);

  fclose(f);

  for (int o = 0, i = n - 1; o < n; ++o, --i) {
    preal sum = bvp[pp[i]];

    for (int j = i + 1; j < n; ++j)
      sum -= yvp[j] * mvp[pp[i] * n + j];

    yvp[i] = sum / mvp[pp[i] * n + i];
  }

  free(pp);
}
