#include <vec.h>

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

struct vec* vec_new(int n) {
  struct vec* vec = malloc(sizeof(struct vec));

  vec->v = malloc(sizeof(real) * n);
  vec->n = n;

  return vec;
}

void vec_rnd(struct vec* v, int u) {
#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < v->n; ++i)
    v->v[i] = rand() % (u + 1);
}

void vec_seq(struct vec* v) {
#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < v->n; ++i)
    v->v[i] = i + 1;
}

void vec_fget(FILE* f, struct vec* a) {
  for (int i = 0; i < a->n; ++i)
    fscanf(f, "%lf", &a->v[i]);
}

void vec_fput(FILE* f, struct vec* a) {
  for (int i = 0; i < a->n; ++i)
    fprintf(f, "%.7e ", a->v[i]);
}

void vec_add(struct vec* a, struct vec* b, struct vec* c) {
#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < a->n; ++i)
    c->v[i] = a->v[i] + b->v[i];
}

void vec_cmb(struct vec* a, struct vec* b, struct vec* c, real k) {
#ifdef OMP_THREADS_NUM
#pragma omp parallel for num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < a->n; ++i)
    c->v[i] = a->v[i] + b->v[i] * k;
}

void vec_mlt(struct vec* a, struct vec* b, real* r) {
  real s = 0;

  for (int i = 0; i < a->n; ++i)
    s += a->v[i] * b->v[i];

  *r = s;
}

void vec_norm(struct vec* a, real* r) {
  real s = 0;

#ifdef OMP_THREADS_NUM
#pragma omp parallel for reduction(+ : s) num_threads(OMP_THREADS_NUM)
#endif  // OMP
  for (int i = 0; i < a->n; ++i)
    s += a->v[i] * a->v[i];

  *r = sqrt(s);
}

void vec_free(struct vec* a) {
  free(a->v);
  free(a);
}
