#ifndef MTX_H
#define MTX_H

#include <stddef.h>
#include <stdio.h>
#include <vec/vec.h>

struct mtx {
  int n;

  double** v;
};

struct mtx* mtx_new(int n);

int mtx_fget(FILE* f, struct mtx* mp);
int mtx_fput(FILE* f, struct mtx* mp);

int mtx_nrm(struct mtx* ap, double* rp);

int mtx_vmlt(struct mtx* ap, struct vec* xp, struct vec* fp);
int mtx_mmlt(struct mtx* ap, struct mtx* bp, struct mtx* cp);

int mtx_cls(struct mtx* mp);

#endif  // MTX_H
