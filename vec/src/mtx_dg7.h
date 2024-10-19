#ifndef MTX_DG7_H
#define MTX_DG7_H

#include <stdio.h>

struct mtx_dg7 {
  int n;
  int m;
  int k;

  real* dp;
  real** lpp;
  real** upp;

  int* s;
};

struct mtx_dg7* mtx_dg7_new(int n, int m, int k);

int mtx_dg7_fget(FILE* f, struct mtx_dg7* mp);
int mtx_dg7_fput(FILE* f, struct mtx_dg7* mp);

int mtx_dg7_ddm(struct mtx_dg7* mp, int k);

void mtx_dg7_free(struct mtx_dg7* mp);

#endif  // MTX_DG7_H
