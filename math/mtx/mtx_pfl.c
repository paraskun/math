#include <def.h>

#ifdef MTX_PFL

#include <mtx/mtx_pfl.h>

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
    double r = a->d[i];

    for (int k = 1; k <= c; ++k)
      r -= a->l[a->p[i] + c - k] * a->u[a->p[i] + c - k] * a->d[i - k];

    a->d[i] = r;

    for (int j = i + 1; j < a->n; ++j) {
      int cc = a->p[j + 1] - a->p[j] - j + i;

      if (cc > -1) {
        double l = a->l[a->p[j] + cc];
        double u = a->u[a->p[j] + cc];

        for (int k = 1; k <= min(cc, c); ++k) {
          l -= a->l[a->p[j] + cc - k] * a->u[a->p[i] + c - k] * a->d[i - k];
          u -= a->l[a->p[i] + c - k] * a->u[a->p[j] + cc - k] * a->d[i - k];
        }

        a->l[a->p[j] + cc] = l / a->d[i];
        a->u[a->p[j] + cc] = u / a->d[i];
      }
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
