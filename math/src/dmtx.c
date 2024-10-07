#include <dmtx.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct dmtx* dmtx_new(int n) {
  struct dmtx* mp = malloc(sizeof(struct dmtx));

  mp->v = malloc(sizeof(real) * n * n);
  mp->n = n;

  return mp;
}

void dmtx_rnd(struct dmtx* mp, int u) {
  int n = mp->n;
  real* vp = mp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n * n; ++i)
    vp[i] = rand() % u;
}

void dmtx_seq(struct dmtx* mp) {
  int n = mp->n;
  real* vp = mp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n * n; ++i)
    vp[i] = i + 1;
}

void dmtx_ddm(struct dmtx* mp, int k) {
  int n = mp->n;
  real* vp = mp->v;

  for (int i = 0, r = 0; i < n; ++i, r += n) {
    real sum = 0;

    for (int j = 0; j < i; ++j) {
      vp[r + j] = -(rand() % 5);
      sum += vp[r + j];
    }

    for (int j = i + 1; j < n; ++j) {
      vp[r + j] = -(rand() % 5);
      sum += vp[r + j];
    }

    if (i == 0)
      vp[r + i] = -sum + 1 / pow(10, k);
    else
      vp[r + i] = -sum;
  }
}

void dmtx_hlb(struct dmtx* mp) {
  int n = mp->n;
  real* vp = mp->v;

  for (int i = 0, r = -1; i < n; ++i, r += n)
    for (int j = 1; j <= n; ++j)
      vp[r + j] = 1.0 / (i + j);
}

void dmtx_fget(FILE* f, struct dmtx* mp) {
  int n = mp->n;
  real* vp = mp->v;

  for (int i = 0; i < n * n; ++i)
    fscanf(f, "%lf", &vp[i]);
}

void dmtx_fput(FILE* f, struct dmtx* mp) {
  int n = mp->n;
  real* vp = mp->v;

  for (int i = 0, r = 0; i < n; ++i, r += n) {
    for (int j = 0; j < n; ++j)
      fprintf(f, "%10.3e ", vp[r + j]);

    fputc('\n', f);
  }
}

void dmtx_mmlt(struct dmtx* ap, struct dmtx* bp, struct dmtx* cp) {
  int n = ap->n;

  real* avp = ap->v;
  real* bvp = bp->v;
  real* cvp = cp->v;

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
}

void dmtx_vmlt(struct dmtx* ap, struct vec* bp, struct vec* cp) {
  int n = ap->n;

  real* avp = ap->v;
  real* bvp = bp->v;
  real* cvp = cp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < ap->n; ++i) {
    int ir = i * n;

    cvp[i] = 0.0;

    for (int j = 0; j < ap->n; ++j)
      cvp[i] += avp[ir + j] * bvp[j];
  }
}

void dmtx_nrm(struct dmtx* mp, real* rp) {
  int n = mp->n;
  real* vp = mp->v;
  real r = 0.0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for reduction(+ : r) num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n * n; ++i)
    r += vp[i] * vp[i];

  *rp = sqrt(r);
}

void dmtx_free(struct dmtx* mp) {
  free(mp->v);
  free(mp);
}
