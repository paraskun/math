#include <def.h>

#ifdef MTX_DNS

#include <mtx_dns.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct mtx* mtx_new(size_t n) {
  struct mtx* m = malloc(sizeof(struct mtx));

  m->v = malloc(sizeof(real) * n * n);
  m->n = n;

  return m;
}

struct mtx* mtx_rnd(size_t n, size_t u) {
  struct mtx* m = mtx_new(n);

#pragma omp parallel for num_threads(THRD)
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j)
      m->v[i * n + j] = rand() % (u + 1);

  return m;
}

struct mtx* mtx_seq(size_t n) {
  struct mtx* m = mtx_new(n);

#pragma omp parallel for num_threads(THRD)
  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j)
      m->v[i * n + j] = j;

  return m;
}

void mtx_fget(const char* fn, struct mtx* a) {
  FILE* f = fopen(fn, "r");

  for (size_t i = 0; i < a->n; ++i)
    for (size_t j = 0; j < a->n; ++j)
      fscanf(f, "%lf", &a->v[i * a->n + j]);

  fclose(f);
}

void mtx_fput(const char* fn, struct mtx* a) {
  FILE* f = fopen(fn, "w");

  for (size_t i = 0; i < a->n; ++i) {
    for (size_t j = 0; j < a->n; ++j)
      fprintf(f, "%.7e ", a->v[i * a->n + j]);

    fputc('\n', f);
  }

  fclose(f);
}

void mtx_cput(struct mtx* a) {
  for (size_t i = 0; i < a->n; ++i) {
    for (size_t j = 0; j < a->n; ++j)
      printf("%.7e ", a->v[i * a->n + j]);

    putchar('\n');
  }
}

void mtx_mlt_mtx(struct mtx* a, struct mtx* b, struct mtx* c) {
#pragma omp parallel for num_threads(THRD)
  for (size_t i = 0; i < a->n; ++i)
    for (size_t j = 0; j < a->n; ++j) {
      c->v[i * a->n + j] = 0.0;

      for (size_t e = 0; e < a->n; ++e)
        c->v[i * a->n + j] += a->v[i * a->n + e] * b->v[e * a->n + j];
    }
}

void mtx_mlt_vec(struct mtx* a, struct vec* b, struct vec* c) {
#pragma omp parallel for num_threads(THRD)
  for (size_t i = 0; i < a->n; ++i) {
    c->v[i] = 0.0;

    for (size_t j = 0; j < a->n; ++j)
      c->v[i] += a->v[i * a->n + j] * b->v[j];
  }
}

void mtx_norm(struct mtx* a, real* r) {
  real s = 0;

#pragma omp parallel for reduction(+ : s) num_threads(THRD)
  for (size_t i = 0; i < a->n; ++i)
    for (size_t j = 0; j < a->n; ++j)
      s += a->v[i * a->n + j] * a->v[i * a->n + j];

  *r = sqrt(s);
}

void mtx_free(struct mtx* a) {
  free(a->v);
  free(a);
}

#endif  // MTX_DNS
