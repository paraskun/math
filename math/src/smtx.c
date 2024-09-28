#ifdef SMTX

#include <mtx.h>

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define min(x, y) (((x) < (y)) ? (x) : (y))

struct mtx* mtx_new(int n, int s) {
  struct mtx* mp = malloc(sizeof(struct mtx));

  mp->n = n;
  mp->s = s;
  mp->p = malloc(sizeof(int) * (n + 1));
  mp->d = malloc(sizeof(real) * n);
  mp->l = malloc(sizeof(real) * s);
  mp->u = malloc(sizeof(real) * s);

  return mp;
}

void mtx_ddm(struct mtx* mp, int k) {
  int n = mp->n;

  real* mlp = mp->l;
  real* mup = mp->u;
  real* mdp = mp->d;
  int* mpp = mp->p;

  int ir = 0;
  preal sum = 0;

  mpp[0] = ir;

  for (int i = 1; i < n; ++i) {
    ir += i - 1;

    mpp[i] = ir;
    mlp[ir] = -(rand() % 4) - 1;
    mup[ir] = -(rand() % 4) - 1;

    sum += mlp[ir] + mup[ir];

    for (int j = 1; j < i; ++j) {
      mlp[ir + j] = -(rand() % 5);
      mup[ir + j] = -(rand() % 5);

      sum += mlp[ir + j] + mup[ir + j];
    }
  }

  mpp[n] = n * (n - 1) / 2;
  mdp[0] = 1.0 / pow(10.0, k) - sum;

  for (int i = 1; i < n; ++i)
    mdp[i] = -sum;
}

void mtx_hlb(struct mtx* mp) {
  int n = mp->n;
  int ir = 0;

  real* mlp = mp->l;
  real* mup = mp->u;
  real* mdp = mp->d;
  int* mpp = mp->p;

  mpp[0] = ir;
  mdp[0] = 1;

  for (int i = 1, i2 = 2; i < n; ++i, i2 += 2) {
    ir += i - 1;

    mpp[i] = ir;
    mdp[i] = 1.0 / (i2 + 1.0);

    for (int j = 0; j < i; ++j) {
      real v = 1.0 / (i + j + 1.0);

      mlp[ir + j] = v;
      mup[ir + j] = v;
    }
  }

  mpp[n] = n * (n - 1) / 2;
}

void mtx_fget(FILE* f, struct mtx* mp) {
  int n = mp->n;
  int s = mp->s;

  real* mlp = mp->l;
  real* mup = mp->u;
  real* mdp = mp->d;
  int* mpp = mp->p;

  for (int i = 0; i <= n; ++i)
    fscanf(f, "%u", &mpp[i]);

  for (int i = 0; i < n; ++i)
    fscanf(f, "%lf", &mdp[i]);

  for (int i = 0; i < s; ++i)
    fscanf(f, "%lf", &mlp[i]);

  for (int i = 0; i < s; ++i)
    fscanf(f, "%lf", &mup[i]);
}

void mtx_fput(FILE* f, struct mtx* mp) {
  int n = mp->n;
  int s = mp->s;

  real* mlp = mp->l;
  real* mup = mp->u;
  real* mdp = mp->d;
  int* mpp = mp->p;

  for (int i = 0; i <= n; ++i)
    fprintf(f, "%u ", mpp[i]);

  fputc('\n', f);

  for (int i = 0; i < n; ++i)
    fprintf(f, "%.7e ", mdp[i]);

  fputc('\n', f);

  for (int i = 0; i < s; ++i)
    fprintf(f, "%.7e ", mlp[i]);

  fputc('\n', f);

  for (int i = 0; i < s; ++i)
    fprintf(f, "%.7e ", mup[i]);
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
