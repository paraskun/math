#include <mtx_dg7.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

struct mtx_dg7* mtx_dg7_new(int n, int m, int k) {
  struct mtx_dg7* mp = malloc(sizeof(struct mtx_dg7));

  mp->n = n;
  mp->m = m;
  mp->k = k;

  mp->s = malloc(sizeof(int) * 3);

  mp->s[0] = n - 1;
  mp->s[1] = mp->s[0] - 1 - m;
  mp->s[2] = mp->s[1] - 1 - k;

  mp->dp = malloc(sizeof(real) * n);
  mp->lpp = malloc(sizeof(real*) * 3);
  mp->upp = malloc(sizeof(real*) * 3);

  for (int i = 0; i < 3; ++i) {
    mp->lpp[i] = malloc(sizeof(real) * mp->s[i]);
    mp->upp[i] = malloc(sizeof(real) * mp->s[i]);
  }

  return mp;
}

int mtx_dg7_fget(FILE* f, struct mtx_dg7* mp) {
  for (int i = 0; i < mp->n; ++i)
    fscanf(f, FMT, &mp->dp[i]);

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < mp->s[i]; ++j)
      fscanf(f, FMT, &mp->lpp[i][j]);

    for (int j = 0; j < mp->s[i]; ++j)
      fscanf(f, FMT, &mp->upp[i][j]);
  }

  return 0;
}

int mtx_dg7_fput(FILE* f, struct mtx_dg7* mp) {
  for (int i = 0; i < mp->n; ++i)
    fprintf(f, "%10.3e ", mp->dp[i]);

  putc('\n', f);

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < mp->s[i]; ++j)
      fprintf(f, "%10.e ", mp->lpp[i][j]);

    putc('\n', f);

    for (int j = 0; j < mp->s[i]; ++j)
      fprintf(f, "%10.e ", mp->upp[i][j]);

    putc('\n', f);
  }

  return 0;
}

int mtx_dg7_ddm(struct mtx_dg7* mp, int k) {
  memset(mp->dp, 0, sizeof(double) * mp->n);

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < mp->s[i]; ++j) {
      mp->lpp[i][j] = -(rand() % 5);
      mp->upp[i][j] = -(rand() % 5);

      mp->dp[j] -= mp->lpp[i][j];
      mp->dp[j + 1] -= mp->upp[i][j];
    }

  mp->dp[0] += 1.0 / pow(10, k);

  return 0;
}

void mtx_dg7_free(struct mtx_dg7* mp) {
  free(mp->dp);

  for (int i = 0; i < 3; ++i) {
    free(mp->lpp[i]);
    free(mp->upp[i]);
  }

  free(mp->lpp);
  free(mp->upp);
  free(mp);
}
