#include <mtx_all.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct mtx* mtx_new(int n) {
  struct mtx* mp = malloc(sizeof(struct mtx));

  mp->vp = malloc(sizeof(double) * n * n);
  mp->n = n;

  return mp;
}

int mtx_fget(FILE* f, struct mtx* mp) {
  int n = mp->n;
  double* vp = mp->vp;

  for (int i = 0; i < n * n; ++i)
    fscanf(f, "%lf", &vp[i]);

  return 0;
}

int mtx_fput(FILE* f, struct mtx* mp) {
  int n = mp->n;
  double* vp = mp->vp;

  for (int i = 0, r = 0; i < n; ++i, r += n) {
    for (int j = 0; j < n; ++j)
      fprintf(f, "%10.3e ", vp[r + j]);

    fputc('\n', f);
  }

  return 0;
}

int mtx_ddm(struct mtx* mp) {
  int n = mp->n;
  double* vp = mp->vp;

  for (int i = 0, r = 0; i < n; ++i, r += n) {
    double sum = 0;

    for (int j = 0; j < i; ++j) {
      vp[r + j] = -(rand() % 5);
      sum += vp[r + j];
    }

    for (int j = i + 1; j < n; ++j) {
      vp[r + j] = -(rand() % 5);
      sum += vp[r + j];
    }

    vp[r + i] = -sum;
  }

  return 0;
}

int mtx_hlb(struct mtx* mp) {
  int n = mp->n;
  double* vp = mp->vp;

  for (int i = 0, r = -1; i < n; ++i, r += n)
    for (int j = 1; j <= n; ++j)
      vp[r + j] = 1.0 / (i + j);

  return 0;
}

int mtx_nrm(struct mtx* mp, double* rp) {
  int n = mp->n;
  double* vp = mp->vp;
  double r = 0.0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for reduction(+ : r) num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n * n; ++i)
    r += vp[i] * vp[i];

  *rp = sqrt(r);

  return 0;
}

int mtx_vmlt(struct mtx* ap, struct vec* bp, struct vec* cp) {
  int n = ap->n;

  double* avp = ap->vp;
  double* bvp = bp->vp;
  double* cvp = cp->vp;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < ap->n; ++i) {
    int ir = i * n;

    double sum = 0.0;

    for (int j = 0; j < ap->n; ++j)
      sum += avp[ir + j] * bvp[j];

    cvp[i] = sum;
  }

  return 0;
}

int mtx_mmlt(struct mtx* ap, struct mtx* bp, struct mtx* cp) {
  int n = ap->n;

  double* avp = ap->vp;
  double* bvp = bp->vp;
  double* cvp = cp->vp;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i) {
    int ir = i * n;

    for (int j = 0, ij = ir; j < n; ++j, ++ij) {
      cvp[ij] = 0.0;

      for (int e = 0, er = 0; e < n; ++e, er += n)
        cvp[ij] += avp[ir + e] * bvp[er + j];
    }
  }

  return 0;
}

int mtx_cls(struct mtx* mp) {
  free(mp->vp);
  free(mp);

  return 0;
}
