#include <mtx_pie.h>

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 10

struct mtx_pie* mtx_pie_new(int n, int s, int c) {
  struct mtx_pie* mp = malloc(sizeof(struct mtx_pie));

  mp->v = malloc(sizeof(real) * n * c);
  mp->n = n;
  mp->s = s;
  mp->c = c;

  return mp;
}

int mtx_pie_fget(FILE* f, struct mtx_pie* mp) {
  fseek(f, mp->s * mp->n * WIDTH, SEEK_SET);

  for (int i = 0; i < mp->n * mp->c; ++i)
    if (fscanf(f, FMT, &mp->v[i]) != 1)
      return -1;

  return 0;
}

int mtx_pie_fput(FILE* f, struct mtx_pie* mp) {
  fseek(f, mp->s * mp->n * WIDTH, SEEK_SET);

  for (int i = mp->s, k = 0; i < mp->s + mp->c; ++i) {
    for (int j = 0; j < mp->n; ++j, ++k) {
      fprintf(f, "%9lf", mp->v[k]);
      fputc(j == mp->n - 1 ? '\n' : ' ', f);
    }
  }

  return 0;
}

int mtx_pie_seq(struct mtx_pie* mp, int s) {
  for (int i = 0, v = s; i < mp->c * mp->n; ++i, ++v)
    mp->v[i] = v;

  return 0;
}

int mtx_pie_vmlt(struct mtx_pie* mp, struct vec* xp, struct vec* bp) {
  int r = 0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = mp->s; i < mp->s + mp->c; ++i) {
    bp->v[i] = 0;

    for (int j = 0; j < mp->n; ++j)
      bp->v[i] += mp->v[r + j] * xp->v[j];

    r += mp->n;
  }

  return 0;
}

int mtx_pie_free(struct mtx_pie* mp) {
  free(mp->v);
  free(mp);

  return 0;
}
