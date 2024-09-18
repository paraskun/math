#include <def.h>

#ifdef MTX_PFL

#include <mtx_pfl.h>

#include <stdio.h>
#include <stdlib.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))

struct mtx* mtx_new(size_t n, size_t s) {
  struct mtx* m = malloc(sizeof(struct mtx));

  m->n = n;
  m->s = s;
  m->p = malloc(sizeof(size_t) * (n + 1));
  m->d = malloc(sizeof(real) * n);
  m->l = malloc(sizeof(real) * s);
  m->u = malloc(sizeof(real) * s);

  return m;
}

void mtx_fget(const char* fn, struct mtx* a) {
  FILE* f = fopen(fn, "r");

  for (size_t i = 0; i <= a->n; ++i)
    fscanf(f, "%lu", &a->p[i]);

  for (size_t i = 0; i < a->n; ++i)
    fscanf(f, "%lf", &a->d[i]);

  for (size_t i = 0; i < a->s; ++i)
    fscanf(f, "%lf", &a->l[i]);

  for (size_t i = 0; i < a->s; ++i)
    fscanf(f, "%lf", &a->u[i]);

  fclose(f);
}

void mtx_fput(const char* fn, struct mtx* a) {
  FILE* f = fopen(fn, "w");

  for (size_t i = 0; i <= a->n; ++i)
    fprintf(f, "%lu ", a->p[i]);

  fputc('\n', f);

  for (size_t i = 0; i < a->n; ++i)
    fprintf(f, "%.7e ", a->d[i]);

  fputc('\n', f);

  for (size_t i = 0; i < a->s; ++i)
    fprintf(f, "%.7e ", a->l[i]);

  fputc('\n', f);

  for (size_t i = 0; i < a->s; ++i)
    fprintf(f, "%.7e ", a->u[i]);

  fclose(f);
}

void mtx_ldu(struct mtx* a) {
  for (int i = 0; i < a->n; ++i) {
    int c = a->p[i + 1] - a->p[i];

    for (int k = 0; k < c; ++k) {
      a->d[i] -= a->l[a->p[i] + c - 1 - k] * a->u[a->p[i] + c - 1 - k] *
                 a->d[i - 1 - k];
    }

    for (int j = i + 1; j < a->n; ++j) {
      int cr = a->p[j + 1] - a->p[j] - j + i;
      int cc = a->p[i + 1] - a->p[i];

      for (int k = 0; k < min(cr, cc); ++k) {
        a->l[a->p[j] + cr] -= a->l[a->p[j] + cr - 1 - k] *
                              a->u[a->p[i] + cc - 1 - k] * a->d[i - 1 - k];
      }

      if (cr > -1)
        a->l[a->p[j] + cr] /= a->d[i];
    }

    for (int j = i + 1; j < a->n; ++j) {
      int cc = a->p[j + 1] - a->p[j] - j + i;
      int cr = a->p[i + 1] - a->p[i];

      for (int k = 0; k < min(cr, cc); ++k) {
        a->u[a->p[j] + cr] -= a->l[a->p[i] + cr - 1 - k] *
                              a->u[a->p[j] + cc - 1 - k] * a->d[i - 1 - k];
      }

      if (cc > -1)
        a->u[a->p[j] + cc] /= a->d[i];
    }
  }
}

void mtx_free(struct mtx* m) {
  free(m->d);
  free(m->l);
  free(m->u);
  free(m->p);
  free(m);
}

#endif  // MTX_PFL
