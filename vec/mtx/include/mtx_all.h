#ifndef DMTX_H
#define DMTX_H

#include <stddef.h>
#include <stdio.h>
#include <vec.h>

struct mtx_all {
  int n;
  double* vp;
};

struct mtx_all* mtx_all_new(int n);

int mtx_all_fget(FILE* f, struct mtx_all* mp);
int mtx_all_fput(FILE* f, struct mtx_all* mp);

int mtx_all_ddm(struct mtx_all* mp);
int mtx_all_hlb(struct mtx_all* mp);

int mtx_all_nrm(struct mtx_all* ap, double* rp);

int mtx_all_vmlt(struct mtx_all* ap, struct vec* xp, struct vec* rp);
int mtx_all_mmlt(struct mtx_all* ap, struct mtx_all* bp, struct mtx_all* cp);

void mtx_all_free(struct mtx_all* mp);

#endif  // MTX_H
