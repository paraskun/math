#include <vec/mtx_pie.h>

#include <stdio.h>
#include <stdlib.h>

#define WIDTH 10

struct mtx_pie* mtx_pie_new(int n, int s, int c) {
  struct mtx_pie* mp = malloc(sizeof(struct mtx_pie));

  mp->vp = malloc(sizeof(double) * n * c);
  mp->n = n;
  mp->sr = s;
  mp->rc = c;

  return mp;
}

int mtx_pie_fget(FILE* f, struct mtx_pie* mp) {
  fseek(f, mp->sr * mp->n * WIDTH, SEEK_SET);

  for (int i = 0; i < mp->n * mp->rc; ++i)
    if (fscanf(f, "%lf", &mp->vp[i]) != 1)
      return -1;

  return 0;
}

int mtx_pie_fput(FILE* f, struct mtx_pie* mp) {
  fseek(f, mp->sr * mp->n * WIDTH, SEEK_SET);

  for (int i = mp->sr, k = 0; i < mp->sr + mp->rc; ++i) {
    for (int j = 0; j < mp->n; ++j, ++k) {
      fprintf(f, "%9lf", mp->vp[k]);
      fputc(j == mp->n - 1 ? '\n' : ' ', f);
    }
  }

  return 0;
}

int mtx_pie_seq(struct mtx_pie* mp, int s) {
  for (int i = 0, v = s; i < mp->rc * mp->n; ++i, ++v)
    mp->vp[i] = v;

  return 0;
}

int mtx_pie_vmlt(struct mtx_pie* mp, struct vec* xp, struct vec* bp) {
  int r = 0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = mp->sr; i < mp->sr + mp->rc; ++i) {
    bp->vp[i] = 0;

    for (int j = 0; j < mp->n; ++j)
      bp->vp[i] += mp->vp[r + j] * xp->vp[j];

    r += mp->n;
  }

  return 0;
}

int mtx_pie_free(struct mtx_pie* mp) {
  free(mp->vp);
  free(mp);

  return 0;
}
