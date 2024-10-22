#include <vec.h>

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

struct vec* vec_new(int n) {
  struct vec* vec = malloc(sizeof(struct vec));

  vec->vp = malloc(sizeof(double) * n);
  vec->n = n;

  return vec;
}

int vec_rnd(struct vec* vp, int u) {
  int n = vp->n;
  double* vvp = vp->vp;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    vvp[i] = rand() % (u + 1);

  return 0;
}

int vec_seq(struct vec* vp, int s) {
  int n = vp->n;
  double* vvp = vp->vp;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0, v = s; i < n; ++i, ++v)
    vvp[i] = v;

  return 0;
}

int vec_fget(FILE* f, struct vec* vp) {
  int n = vp->n;
  double* vvp = vp->vp;

  for (int i = 0; i < n; ++i)
    if (fscanf(f, "%lf", &vvp[i]) != 1)
      return -1;

  return 0;
}

int vec_fput(FILE* f, struct vec* vp) {
  int n = vp->n;
  double* vvp = vp->vp;

  for (int i = 0; i < n; ++i)
    if (fprintf(f, "%.3e ", vvp[i]) < 0)
      return -1;

  return 0;
}

int vec_add(struct vec* ap, struct vec* bp, struct vec* rp) {
  int n = ap->n;

  double* avp = ap->vp;
  double* bvp = bp->vp;
  double* rvp = rp->vp;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    rvp[i] = avp[i] + bvp[i];

  return 0;
}

int vec_sub(struct vec* ap, struct vec* bp, struct vec* rp) {
  int n = ap->n;

  double* avp = ap->vp;
  double* bvp = bp->vp;
  double* rvp = rp->vp;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    rvp[i] = avp[i] - bvp[i];

  return 0;
}

int vec_cmb(struct vec* ap, struct vec* bp, struct vec* rp, double k) {
  int n = ap->n;

  double* avp = ap->vp;
  double* bvp = bp->vp;
  double* rvp = rp->vp;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    rvp[i] = avp[i] + bvp[i] * k;

  return 0;
}

int vec_mlt(struct vec* ap, struct vec* bp, double* rp) {
  int n = ap->n;

  double* avp = ap->vp;
  double* bvp = bp->vp;
  double s = 0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for reduction(+ : s) num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    s += avp[i] * bvp[i];

  *rp = s;
  return 0;
}

int vec_nrm(struct vec* vp, double* rp) {
  int n = vp->n;

  double* vvp = vp->vp;
  double s = 0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for reduction(+ : s) num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < n; ++i)
    s += vvp[i] * vvp[i];

  *rp = sqrt(s);
  return 0;
}

void vec_free(struct vec* vp) {
  free(vp->vp);
  free(vp);
}
