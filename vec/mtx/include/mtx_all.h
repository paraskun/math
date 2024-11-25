#ifndef DMTX_H
#define DMTX_H

#include <stddef.h>
#include <stdio.h>
#include <vec.h>

struct mtx {
  int n;
  double* vp;
};

struct mtx* mtx_new(int n);

int mtx_fget(FILE* f, struct mtx* mp);
int mtx_fput(FILE* f, struct mtx* mp);

int mtx_ddm(struct mtx* mp);
int mtx_hlb(struct mtx* mp);

int mtx_nrm(struct mtx* ap, double* rp);

int mtx_vmlt(struct mtx* ap, struct vec* xp, struct vec* rp);
int mtx_mmlt(struct mtx* ap, struct mtx* bp, struct mtx* cp);

int mtx_cls(struct mtx* mp);

#endif  // MTX_H
