#include <def.h>
#include <math.h>
#include <stdint.h>

#ifdef SMTX

#include <mtx/smtx.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>

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

static int rnd() {
  uint8_t rnd = 255;

  while (rnd > 254)
    if (getrandom(&rnd, 1, 0) == -1)
      exit(-1);

  return rnd % 5 - 4;
}

struct mtx* mtx_newdd(size_t n, size_t k) {
  size_t s = n * (n - 1) / 2;
  double sum = 0;

  struct mtx* m = mtx_new(n, s);

  m->p[0] = 0;

  for (size_t i = 1; i < n; ++i) {
    size_t bgn = (1 + i - 1) / 2 * (i - 1);

    m->p[i] = bgn;
    m->l[bgn] = rnd();
    m->u[bgn] = rnd();

    while (m->l[bgn] == 0)
      m->l[bgn] = rnd();

    while (m->u[bgn] == 0)
      m->u[bgn] = rnd();

    sum += m->l[bgn] + m->u[bgn];

    for (size_t j = 1; j < i; ++j) {
      m->l[bgn + j] = rnd();
      m->u[bgn + j] = rnd();

      sum += m->l[bgn + j] + m->u[bgn + j];
    }
  }

  m->p[n] = s;
  m->d[0] = 1.0 / pow(10.0, k) - sum;

  for (size_t i = 1; i < n; ++i)
    m->d[i] = -sum;

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
    preal r = a->d[i];

    for (int k = 1; k <= c; ++k)
      r -= a->l[a->p[i] + c - k] * a->u[a->p[i] + c - k] * a->d[i - k];

    a->d[i] = r;

    for (int j = i + 1; j < a->n; ++j) {
      int cc = a->p[j + 1] - a->p[j] - j + i;

      if (cc > -1) {
        preal l = a->l[a->p[j] + cc];
        preal u = a->u[a->p[j] + cc];

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
