#include <mtx_all.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct mtx_all* mtx_all_new(int n) {
  struct mtx_all* mp = malloc(sizeof(struct mtx_all));

  mp->vp = malloc(sizeof(double) * n * n);
  mp->n = n;

  return mp;
}

int mtx_all_fget(FILE* f, struct mtx_all* mp) {
  int n = mp->n;
  double* vp = mp->vp;

  for (int i = 0; i < n * n; ++i)
    fscanf(f, "%lf", &vp[i]);

  return 0;
}

int mtx_all_fput(FILE* f, struct mtx_all* mp) {
  int n = mp->n;
  double* vp = mp->vp;

  for (int i = 0, r = 0; i < n; ++i, r += n) {
    for (int j = 0; j < n; ++j)
      fprintf(f, "%10.3e ", vp[r + j]);

    fputc('\n', f);
  }

  return 0;
}

int mtx_all_ddm(struct mtx_all* mp) {
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

int mtx_all_hlb(struct mtx_all* mp) {
  int n = mp->n;
  double* vp = mp->vp;

  for (int i = 0, r = -1; i < n; ++i, r += n)
    for (int j = 1; j <= n; ++j)
      vp[r + j] = 1.0 / (i + j);

  return 0;
}

int mtx_all_nrm(struct mtx_all* mp, double* rp) {
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

int mtx_all_vmlt(struct mtx_all* ap, struct vec* bp, struct vec* cp) {
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

int mtx_all_mmlt(struct mtx_all* ap, struct mtx_all* bp, struct mtx_all* cp) {
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

void mtx_all_free(struct mtx_all* mp) {
  free(mp->vp);
  free(mp);
}
