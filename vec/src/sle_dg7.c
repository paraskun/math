#include <sle_dg7.h>

#include <stdlib.h>

static int step(struct args* a, struct vec* rp) {
  int n = a->xp->n;

  return 0;
}

int sle_dg7_jac(struct args a, struct props p) {
  return iter(&a, &p, a.xp->v);
}

int sle_dg7_rx(struct args a, struct props p) {}

int sle_dg7_brx(struct args, struct props);
