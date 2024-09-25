#include <def.h>

#ifdef SMTX

#include <mtx/smtx.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))

struct mtx* mtx_new(size_t n, size_t s) {
  struct mtx* mp = malloc(sizeof(struct mtx));

  mp->n = n;
  mp->s = s;
  mp->p = malloc(sizeof(size_t) * (n + 1));
  mp->d = malloc(sizeof(real) * n);
  mp->l = malloc(sizeof(real) * s);
  mp->u = malloc(sizeof(real) * s);

  return mp;
}

static int rnd() {
  uint8_t rnd = 255;

  while (rnd > 254)
    if (getrandom(&rnd, 1, 0) == -1)
      exit(-1);

  return rnd % 5 - 4;
}

void mtx_ddm(struct mtx* mp, size_t k) {
  size_t s = mp->n * (mp->n - 1) / 2;
  size_t bgn = 0;
  double sum = 0;

  mp->p[0] = bgn;

  for (size_t i = 1; i < mp->n; ++i) {
    bgn += i - 1;

    mp->p[i] = bgn;
    mp->l[bgn] = rnd();
    mp->u[bgn] = rnd();

    while (mp->l[bgn] == 0)
      mp->l[bgn] = rnd();

    while (mp->u[bgn] == 0)
      mp->u[bgn] = rnd();

    sum += mp->l[bgn] + mp->u[bgn];

    for (size_t j = 1; j < i; ++j) {
      mp->l[bgn + j] = rnd();
      mp->u[bgn + j] = rnd();

      sum += mp->l[bgn + j] + mp->u[bgn + j];
    }
  }

  mp->p[mp->n] = s;
  mp->d[0] = 1.0 / pow(10.0, k) - sum;

  for (size_t i = 1; i < mp->n; ++i)
    mp->d[i] = -sum;
}

void mtx_hlb(struct mtx* mp) {
  size_t s = mp->n * (mp->n - 1) / 2;
  size_t bgn = 0;

  mp->p[0] = bgn;
  mp->d[0] = 1;

  for (size_t i = 1; i < mp->n; ++i) {
    bgn += i - 1;

    mp->p[i] = bgn;
    mp->d[i] = 1.0 / (2.0 * i + 1.0);

    for (size_t j = 0; j < i; ++j) {
      real v = 1.0 / (i + j + 1.0);

      mp->l[bgn + j] = v;
      mp->u[bgn + j] = v;
    }
  }

  mp->p[mp->n] = s;
}

void mtx_fget(FILE* f, struct mtx* a) {
  for (size_t i = 0; i <= a->n; ++i)
    fscanf(f, "%lu", &a->p[i]);

  for (size_t i = 0; i < a->n; ++i)
    fscanf(f, "%lf", &a->d[i]);

  for (size_t i = 0; i < a->s; ++i)
    fscanf(f, "%lf", &a->l[i]);

  for (size_t i = 0; i < a->s; ++i)
    fscanf(f, "%lf", &a->u[i]);
}

void mtx_fput(FILE* f, struct mtx* a) {
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
