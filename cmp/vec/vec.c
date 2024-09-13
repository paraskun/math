#include "vec.h"

#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct vec* vec_new(uint32_t n) {
  struct vec* vec = malloc(sizeof(struct vec));

  vec->v = malloc(sizeof(double) * n);
  vec->n = n;

  return vec;
}

struct vec* vec_rnd(uint32_t n, uint32_t u) {
  struct vec* vec = vec_new(n);

#pragma omp parallel for
  for (uint32_t i = 0; i < n; ++i)
    vec->v[i] = rand() % (u + 1);

  return vec;
}

struct vec* vec_seq(uint32_t n) {
  struct vec* vec = vec_new(n);

#pragma omp parallel for
  for (uint32_t i = 0; i < n; ++i)
    vec->v[i] = i;

  return vec;
}

void vec_fget(FILE* f, struct vec* a) {
  for (uint32_t i = 0; i < a->n; ++i)
    fscanf(f, "%lf", &a->v[i]);
}

void vec_fput(FILE* f, struct vec* a) {
  for (uint32_t i = 0; i < a->n; ++i)
    fprintf(f, "%.3e ", a->v[i]);

  fputc('\n', f);
}

void vec_cput(struct vec* a) {
  printf("(");

  for (uint32_t i = 0; i < a->n; ++i)
    printf("%.1lf, ", a->v[i]);

  printf("\b\b)");
}

void vec_add(struct vec* a, struct vec* b, struct vec* c) {
#pragma omp parallel for
  for (uint32_t i = 0; i < a->n; ++i)
    c->v[i] = a->v[i] + b->v[i];
}

void vec_cmb(struct vec* a, struct vec* b, struct vec* c, double k) {
#pragma omp parallel for num_threads(12)
  for (uint32_t i = 0; i < a->n; ++i)
    c->v[i] = a->v[i] + b->v[i] * k;
}

void vec_mlt(struct vec* a, struct vec* b, double* r) {
  double s = 0;

#pragma omp parallel for reduction(+ : s) num_threads(12)
  for (uint32_t i = 0; i < a->n; ++i)
    s += a->v[i] * b->v[i];

  *r = s;
}

void vec_norm(struct vec* a, double* r) {
  double s = 0;

#pragma omp parallel for reduction(+ : s)
  for (uint32_t i = 0; i < a->n; ++i)
    s += a->v[i] * a->v[i];

  *r = sqrt(s);
}

void vec_free(struct vec* a) {
  free(a->v);
  free(a);
}
