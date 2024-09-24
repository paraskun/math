#include <def.h>
#include <vec/vec.h>

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

struct vec* vec_new(size_t n) {
  struct vec* vec = malloc(sizeof(struct vec));

  vec->v = malloc(sizeof(real) * n);
  vec->n = n;

  return vec;
}

struct vec* vec_rnd(size_t n, size_t u) {
  struct vec* vec = vec_new(n);

#pragma omp parallel for num_threads(TN)
  for (size_t i = 0; i < n; ++i)
    vec->v[i] = rand() % (u + 1);

  return vec;
}

struct vec* vec_seq(size_t n) {
  struct vec* vec = vec_new(n);

#pragma omp parallel for num_threads(TN)
  for (size_t i = 0; i < n; ++i)
    vec->v[i] = i;

  return vec;
}

void vec_fget(const char* fn, struct vec* a) {
  FILE* f = fopen(fn, "r");

  for (size_t i = 0; i < a->n; ++i)
    fscanf(f, "%lf", &a->v[i]);

  fclose(f);
}

void vec_fput(const char* fn, struct vec* a) {
  FILE* f = fopen(fn, "w");

  for (size_t i = 0; i < a->n; ++i)
    fprintf(f, "%.7e ", a->v[i]);

  fclose(f);
}

void vec_cput(struct vec* a) {
  for (size_t i = 0; i < a->n; ++i)
    printf("%.7e ", a->v[i]);
}

void vec_add(struct vec* a, struct vec* b, struct vec* c) {
#pragma omp parallel for num_threads(TN)
  for (size_t i = 0; i < a->n; ++i)
    c->v[i] = a->v[i] + b->v[i];
}

void vec_cmb(struct vec* a, struct vec* b, struct vec* c, real k) {
#pragma omp parallel for num_threads(TN)
  for (size_t i = 0; i < a->n; ++i)
    c->v[i] = a->v[i] + b->v[i] * k;
}

void vec_mlt(struct vec* a, struct vec* b, real* r) {
  real s = 0;

#pragma omp parallel for reduction(+ : s) num_threads(TN)
  for (size_t i = 0; i < a->n; ++i)
    s += a->v[i] * b->v[i];

  *r = s;
}

void vec_norm(struct vec* a, real* r) {
  real s = 0;

  for (size_t i = 0; i < a->n; ++i)
    s += a->v[i] * a->v[i];

  *r = sqrt(s);
}

void vec_free(struct vec* a) {
  free(a->v);
  free(a);
}
