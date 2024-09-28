#ifndef SMTX

#include <math.h>
#include <mtx.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct mtx* mtx_new(int n) {
  struct mtx* mp = malloc(sizeof(struct mtx));

  mp->v = malloc(sizeof(real) * n * n);
  mp->n = n;

  return mp;
}

void mtx_rnd(struct mtx* mp, int u) {
  int n = mp->n;
  real* vp = mp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n * n; ++i)
    vp[i] = rand() % u;
}

void mtx_seq(struct mtx* mp) {
  int n = mp->n;
  real* vp = mp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n * n; ++i)
    vp[i] = i + 1;
}

void mtx_ddm(struct mtx* mp, int k) {
  int n = mp->n;
  real* vp = mp->v;

  real sum = 0;

  for (int i = 0, r = 0; i < n; ++i, r += n)
    for (int j = 0, c = 0; j < i; ++j, c += n) {
      int e = -(rand() % 5);
      sum += e;
      vp[r + j] = e;

      e = -(rand() % 5);
      sum += e;
      vp[c + i] = e;
    }

  vp[0] = -sum + 1 / pow(10, k);

  for (int i = 1, r = n; i < n; ++i, r += n)
    vp[r + i] = -sum;
}

void mtx_hlb(struct mtx* mp) {
  int n = mp->n;
  real* vp = mp->v;

  for (int i = 0, r = -1; i < n; ++i, r += n)
    for (int j = 1; j <= n; ++j)
      vp[r + j] = 1.0 / (i + j);
}

void mtx_fget(FILE* f, struct mtx* mp) {
  int n = mp->n;
  real* vp = mp->v;

  for (int i = 0; i < n * n; ++i)
    fscanf(f, "%lf", &vp[i]);
}

void mtx_fput(FILE* f, struct mtx* mp) {
  int n = mp->n;
  real* vp = mp->v;

  for (int i = 0, r = 0; i < n; ++i, r += n) {
    for (int j = 0; j < n; ++j)
      fprintf(f, "%.7e ", vp[r + j]);

    fputc('\n', f);
  }
}

void mtx_mmlt(struct mtx* ap, struct mtx* bp, struct mtx* cp) {
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

void mtx_vmlt(struct mtx* ap, struct vec* bp, struct vec* cp) {
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

void mtx_norm(struct mtx* mp, real* rp) {
  int n = mp->n;
  real* vp = mp->v;

  for (int i = 0; i < n * n; ++i)
    *rp += vp[i] * vp[i];

  *rp = sqrt(*rp);
}

void mtx_free(struct mtx* mp) {
  free(mp->v);
  free(mp);
}

#endif  // DMTX
