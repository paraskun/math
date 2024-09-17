#include <def.h>
#include <stdint.h>
#include <stdio.h>

#ifdef MTX_PFL_H

#include <mtx_pfl.h>
#include <stdlib.h>

struct mtx* mtx_new(uint32_t n, uint32_t s) {
  struct mtx* m = malloc(sizeof(struct mtx));

  m->n = n;
  m->s = s;
  m->p = malloc(sizeof(uint32_t) * (n + 1));
  m->d = malloc(sizeof(real) * n);
  m->l = malloc(sizeof(real) * s);
  m->u = malloc(sizeof(real) * s);

  return m;
}

void mtx_fget(FILE* f, struct mtx* a) {
  for (uint32_t i = 0; i <= a->n; ++i)
    fscanf(f, "%u", &a->p[i]);

  for (uint32_t i = 0; i < a->n; ++i)
    fscanf(f, "%lf", &a->d[i]);

  for (uint32_t i = 0; i < a->s; ++i)
    fscanf(f, "%lf", &a->l[i]);

  for (uint32_t i = 0; i < a->s; ++i)
    fscanf(f, "%lf", &a->u[i]);
}

void mtx_fput(FILE* f, struct mtx* a) {
  for (uint32_t i = 0; i <= a->n; ++i)
    fprintf(f, "%u ", a->p[i]);

  fputc('\n', f);

  for (uint32_t i = 0; i < a->n; ++i)
    fprintf(f, "%.4e ", a->d[i]);

  fputc('\n', f);

  for (uint32_t i = 0; i < a->s; ++i)
    fprintf(f, "%.4e ", a->l[i]);

  fputc('\n', f);

  for (uint32_t i = 0; i < a->s; ++i)
    fprintf(f, "%.4e ", a->u[i]);
}

void mtx_ldu(struct mtx* a) {}

void mtx_free(struct mtx* m) {
  free(m->d);
  free(m->l);
  free(m->u);
  free(m->p);
  free(m);
}

#endif  // MTX_PFL_H
