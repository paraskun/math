#ifndef SLE_DG7_H
#define SLE_DG7_H

#include <mtx_dg7.h>
#include <vec.h>

struct args {
  struct mtx_dg7* mp;

  struct vec* xp;
  struct vec* fp;
};

struct props {
  real omg;
  real eps;

  int mk;
};

int sle_dg7_jac(struct args, struct props);

int sle_dg7_rx(struct args, struct props);

int sle_dg7_brx(struct args, struct props);

#endif  // SLE_DG7_H
