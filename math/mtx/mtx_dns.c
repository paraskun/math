#ifdef MTX_DNS_H

#include <mtx_dns.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#ifndef THRD
#define THRD 1
#endif

struct mtx* mtx_new(uint32_t n) {
  struct mtx* m = malloc(sizeof(struct mtx));

  m->v = malloc(sizeof(double) * n * n);
  m->n = n;

  return m;
}

struct mtx* mtx_rnd(uint32_t n, uint32_t u) {
  struct mtx* m = mtx_new(n);

#pragma omp parallel for num_threads(THRD)
  for (uint32_t i = 0; i < n; ++i)
    for (uint32_t j = 0; j < n; ++j)
      m->v[i * n + j] = rand() % (u + 1);

  return m;
}

struct mtx* mtx_seq(uint32_t n) {
  struct mtx* m = mtx_new(n);

#pragma omp parallel for num_threads(THRD)
  for (uint32_t i = 0; i < n; ++i)
    for (uint32_t j = 0; j < n; ++j)
      m->v[i * n + j] = j;

  return m;
}

struct mtx* mtx_sequ(uint32_t n) {
  struct mtx* m = mtx_new(n);

#pragma omp parallel for num_threads(THRD)
  for (uint32_t i = 0; i < n; ++i)
    for (uint32_t j = 0; j < n; ++j)
      m->v[i * n + j] = j >= i ? n - j : 0.0;

  return m;
}

void mtx_fget(FILE* f, struct mtx* a) {
  for (uint32_t i = 0; i < a->n; ++i)
    for (uint32_t j = 0; j < a->n; ++j)
      fscanf(f, "%lf", &a->v[i * a->n + j]);
}

void mtx_fput(FILE* f, struct mtx* a) {
  for (uint32_t i = 0; i < a->n; ++i) {
    for (uint32_t j = 0; j < a->n; ++j)
      fprintf(f, "%lf ", a->v[i * a->n + j]);

    fputc('\n', f);
  }
}

void mtx_cput(struct mtx* a) {
  for (uint32_t i = 0; i < a->n; ++i) {
    for (uint32_t j = 0; j < a->n; ++j)
      printf("%4.1lf ", a->v[i * a->n + j]);

    putchar('\n');
  }
}

void mtx_mmlt(struct mtx* a, struct mtx* b, struct mtx* c) {
#pragma omp parallel for num_threads(THRD)
  for (uint32_t i = 0; i < a->n; ++i)
    for (uint32_t j = 0; j < a->n; ++j) {
      c->v[i * a->n + j] = 0.0;

      for (uint32_t e = 0; e < a->n; ++e)
        c->v[i * a->n + j] += a->v[i * a->n + e] * b->v[e * a->n + j];
    }
}

void mtx_vmlt(struct mtx* a, struct vec* b, struct vec* c) {
#pragma omp parallel for num_threads(THRD)
  for (uint32_t i = 0; i < a->n; ++i) {
    c->v[i] = 0.0;

    for (uint32_t j = 0; j < a->n; ++j)
      c->v[i] += a->v[i * a->n + j] * b->v[j];
  }
}

void mtx_norm(struct mtx* a, double* r) {
  double s = 0;

#pragma omp parallel for reduction(+ : s) num_threads(THRD)
  for (uint32_t i = 0; i < a->n; ++i)
    for (uint32_t j = 0; j < a->n; ++j)
      s += a->v[i * a->n + j] * a->v[i * a->n + j];

  *r = sqrt(s);
}

void mtx_free(struct mtx* a) {
  free(a->v);
  free(a);
}

#endif  // MTX_DNS_H
