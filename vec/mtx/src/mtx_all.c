#include <mtx.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct mtx_dns* mtx_dns_new(int n) {
  struct mtx_dns* mp = malloc(sizeof(struct mtx_dns));

  mp->v = malloc(sizeof(real) * n * n);
  mp->n = n;

  return mp;
}

void mtx_dns_rnd(struct mtx_dns* mp, int u) {
  int n = mp->n;
  real* vp = mp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n * n; ++i)
    vp[i] = rand() % u;
}

void mtx_dns_seq(struct mtx_dns* mp) {
  int n = mp->n;
  real* vp = mp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n * n; ++i)
    vp[i] = i + 1;
}

void mtx_dns_ddm(struct mtx_dns* mp, int k) {
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

void mtx_dns_hlb(struct mtx_dns* mp) {
  int n = mp->n;
  real* vp = mp->v;

  for (int i = 0, r = -1; i < n; ++i, r += n)
    for (int j = 1; j <= n; ++j)
      vp[r + j] = 1.0 / (i + j);
}

void mtx_dns_fget(FILE* f, struct mtx_dns* mp) {
  int n = mp->n;
  real* vp = mp->v;

  for (int i = 0; i < n * n; ++i)
    fscanf(f, FMT, &vp[i]);
}

void mtx_dns_fput(FILE* f, struct mtx_dns* mp) {
  int n = mp->n;
  real* vp = mp->v;

  for (int i = 0, r = 0; i < n; ++i, r += n) {
    for (int j = 0; j < n; ++j)
      fprintf(f, "%10.3e ", vp[r + j]);

    fputc('\n', f);
  }
}

void mtx_dns_mmlt(struct mtx_dns* ap, struct mtx_dns* bp, struct mtx_dns* cp) {
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

void mtx_dns_vmlt(struct mtx_dns* ap, struct vec* bp, struct vec* cp) {
  int n = ap->n;

  real* avp = ap->v;
  real* bvp = bp->v;
  real* cvp = cp->v;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < ap->n; ++i) {
    int ir = i * n;

    synt sum = 0.0;

    for (int j = 0; j < ap->n; ++j)
      sum += avp[ir + j] * bvp[j];

    cvp[i] = sum;
  }
}

void mtx_dns_nrm(struct mtx_dns* mp, real* rp) {
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

void mtx_dns_free(struct mtx_dns* mp) {
  free(mp->v);
  free(mp);
}
