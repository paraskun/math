#include <sle_dg7.h>

typedef int (*fn)(struct args*, real*);

static int iter(struct args* a, fn f, real* d, real om) {
  int n = a->xp->n;

  real* xvp = a->xp->v;
  real* mdp = a->mp->dp;

  f(a, d);

  for (int i = 0; i < n; ++i)
    xvp[i] = xvp[i] + (om / mdp[i]) * d[i];

  return 0;
}

static int jac(struct args* a, real* d) {
  int n = a->xp->n;

  real* fvp = a->fp->v;
  real* xvp = a->xp->v;

  for (int j = 0; j < n; ++j) {}

  return 0;
}

int sle_dg7_jac(struct args, struct props) {}

int sle_dg7_rx(struct args, struct props);

int sle_dg7_brx(struct args, struct props);
