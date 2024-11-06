#ifndef MTX_CSR_H
#define MTX_CSR_H

#include <vec.h>

#include <stdio.h>

struct mtx_csj {
  int n;

  int le;
  int ue;

  double* dr;
  double* lr;
  double* ur;

  int* il;
  int* jl;
  int* iu;
  int* ju;
};

struct mtx_csj* mtx_csj_new(int n, int le, int ue);

int mtx_csj_fget(FILE* f, struct mtx_csj* mp);

int mtx_csj_ilu(struct mtx_csj* mp, struct mtx_csj* rp);

int mtx_csj_vmlt(struct mtx_csj* mp, struct vec* xp, struct vec* fp);

void mtx_csj_free(struct mtx_csj* mp);

#endif  // MTX_CSR_H
