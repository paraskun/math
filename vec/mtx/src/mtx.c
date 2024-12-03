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

int mtx_fget(FILE* f, struct mtx* ap) {
  int n = ap->n;
  double** v = ap->v;

  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      fscanf(f, "%lf", &v[i][j]);

  return 0;
}

int mtx_fput(FILE* f, struct mtx* ap) {
  int n = ap->n;
  double** v = ap->v;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j)
      fprintf(f, "%10.3e ", v[i][j]);

    fputc('\n', f);
  }

  return 0;
}

int mtx_nrm(struct mtx* ap, double* rp) {
  int n = ap->n;
  double** v = ap->v;
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

int mtx_vmlt(struct mtx* ap, struct vec* xp, struct vec* fp) {
  int n = ap->n;

  double** av = ap->v;
  double* xv = xp->vp;
  double* fv = fp->vp;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i) {
    double s = 0;

    for (int j = 0; j < n; ++j)
      s += av[i][j] * xv[j];

    fv[i] = s;
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

int mtx_cls(struct mtx* ap) {
  int n = ap->n;

  for (int i = 0; i < n; ++i)
    free(ap->v[i]);

  free(ap->v);
  free(ap);

  return 0;
}
