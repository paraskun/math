#include <vec/mtx.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct mtx* mtx_new(int n) {
  struct mtx* mp = malloc(sizeof(struct mtx));

  mp->n = n;
  mp->v = malloc(sizeof(double*) * n);

  for (int i = 0; i < n; ++i)
    mp->v[i] = malloc(sizeof(double) * n);

  return mp;
}

int mtx_fget(FILE* f, struct mtx* mp) {
  int n = mp->n;
  double** v = mp->v;

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      fscanf(f, "%lf", &v[i][j]);

  return 0;
}

int mtx_fput(FILE* f, struct mtx* mp) {
  int n = mp->n;
  double** v = mp->v;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j)
      fprintf(f, "%10.3e ", v[i][j]);

    fputc('\n', f);
  }

  return 0;
}

int mtx_nrm(struct mtx* mp, double* rp) {
  int n = mp->n;
  double** v = mp->v;
  double r = 0.0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for reduction(+ : r) num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      r += v[i][j] * v[i][j];

  *rp = sqrt(r);

  return 0;
}

int mtx_vmlt(struct mtx* ap, struct vec* bp, struct vec* cp) {
  int n = ap->n;

  double** av = ap->v;
  double* bv = bp->vp;
  double* cv = cp->vp;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i) {
    double s = 0;

    for (int j = 0; j < n; ++j)
      s += av[i][j] * bv[j];

    cv[i] = s;
  }

  return 0;
}

int mtx_mmlt(struct mtx* ap, struct mtx* bp, struct mtx* cp) {
  int n = ap->n;

  double** av = ap->v;
  double** bv = bp->v;
  double** cv = cp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      double s = 0;

      for (int e = 0, er = 0; e < n; ++e, er += n)
        s += av[i][e] * bv[e][i];

      cv[i][j] = s;
    }
  }

  return 0;
}

int mtx_cls(struct mtx* mp) {
  int n = mp->n;

  for (int i = 0; i < n; ++i)
    free(mp->v[i]);

  free(mp->v);
  free(mp);

  return 0;
}
