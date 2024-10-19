#ifndef MTX_DGL_H
#define MTX_DGL_H

#include <stdio.h>

struct mtx_dgl {
  int n;
  int m;
  int k;

  real** dgl;
};

struct mtx_dgl mtx_dgl_new(int n, int m);

int mtx_dgl_fget(FILE* f, struct mtx_dgl* mp);

#endif  // MTX_DGL_H
